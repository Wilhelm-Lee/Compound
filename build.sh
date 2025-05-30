#!/bin/bash

# This file is part of Compound library.
# Copyright (C) 2024-TODAY  William Lee
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
# 
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, see
# <https://www.gnu.org/licenses/>.

# PLEASE PLACE THIS SCRIPT AT THE ROOT DIRECTORY OF TARGETED PROJECT.

RESET=0
DIM=2
FRED=31
FGRE=32
FYEL=33
FCYA=36
WHOAMI="$(whoami)"
PROJECT="$(basename $PWD)"
HEADER_SYSTEMINFO="[ $(uname -o) ($(uname -r)) |"
HEADER_PROJECTNAME="$PROJECT"
HEADER_DATETIME="| $(date) ]"
SCRIPT_DIRECTORY=".build.sh.d"
OUTPUT="$SCRIPT_DIRECTORY/OUTPUT"
ERROR="$SCRIPT_DIRECTORY/ERROR"
NORMALISED_PROJECT_NAME="$(echo "$PROJECT" | awk '{ print tolower($0); }' | sed 's/ /_/g')"
SHAREDOBJECT="lib$NORMALISED_PROJECT_NAME.so"
EXECUTABLE="$NORMALISED_PROJECT_NAME"
USAGE=\
"
Usage:
$0 [OPTION...] - Execute a single building procedure with given sources.

  --sources <file1,file2,...,fileN>
  --compiler <compiler_path>
  --flags <flag1,flag2,...,flagN>
  --shared    # Target will be compiled as a shared object (.so).
  --outdir <dir>
  --bindir <dir>
  --install   # Install headers and target.
    --preinstall-src <dir>
    --preinstall-dst <dir>
    --postinstall-src <dir>
    --postinstall-dst <dir>
  --complain  # Display output from each stage.
  --link <lib>
    --linkdir <dir>
  --output    # Display generated files.
  --dumpenv   # Display script parameters.
  --color     # Print with color.

Example:
  # Building a shared object.
  ./build.sh --sources src/* --shared --complain --outdir out/ --bindir out/bin/
  
  # Building a regualr executable.
  ./build.sh --sources main.c,src/* --complain --link ncurses --compiler /usr/bin/clang
"

sources=""
compiler="/usr/bin/gcc"
flags=""
shared=false
outdir="out"
bindir="$outdir/bin"
install=false
preinstall_src="inc"
preinstall_dst="/usr/include/$PROJECT"
postinstall_src="$bindir"
postinstall_dst="/usr/lib"
complain=false
link=""
linkdir="/usr/lib"
output=false
dumpenv=false
color=false

elapse_begin=""
elapse_nano_begin=""
elapse_end=""
elapse_nano_end=""
elapse_totalseconds=""
elapse_calchours=""
elapse_calcminutes=""
elapse_calcseconds=""
elapse_calcmilliseconds=""
output_files=""
has_error=false

Dye()
{
  while IFS= read -r line; do
    if $color; then
      printf "\e[%sm%s\e[%s""m\n" "$1" "$line" "$RESET"
    else
      echo "$line"
    fi
  done
}

Echo()
{
  while IFS= read line; do echo "$1$line$2"; done
}

Length()
{
  while IFS= read line; do
    echo "$line" | awk '{ print length($0); }'
  done
}

PrintFor()
{
  for i in $(seq 1 $1); do
    printf "%s" "$2"
  done
}

ResetFileBuffer()
{
  printf '' > "$OUTPUT"
  [ "$WHOAMI" = "root" ] && chmod 777 "$OUTPUT"
  printf '' > "$ERROR"
  [ "$WHOAMI" = "root" ] && chmod 777 "$ERROR"
}

RemoveLeadingZero()
{
  string="$1"
  
  while [ -n "$string" ] && [ "${string:0:1}" = '0' ]; do
    string="${string:1}"
  done
  
  echo "$string"
}

HasError()
{
  $has_error && return 1
  
  return 0
}

ReportError()
{
  has_error=true
  printf "%s" "  $1" >&2
}

PrintUsage()
{
  ReportError "$USAGE"
}

PrintStatus()
{
  [ $1 -eq 0 ] && echo "okay" | Dye $FGRE | Echo '  '
 
  ! [ $1 -eq 0 ] && echo "failed" | Dye $FRED | Echo '  '
}

InsufficientArgument()
{
  ReportError "\"$(echo $1 | Dye $FRED)\" requires an argument."
}

RuntimeError()
{
  ReportError
}

FileDoesNotExist()
{
  ReportError "File \"$(echo $1 | Dye $FRED)\" does not exist."
}

DirectoryDoesNotExist()
{
  ReportError "Directory \"$(echo $1 | Dye $FRED)\" does not exist."
}

EmptySources()
{
  ReportError "No source files were given."
}

NoFileCollected()
{
  ReportError "No file was found in \"$(echo $1 | Dye $FRED)\"."
}

AnnounceStage()
{
  ResetFileBuffer
  echo
  echo " $(echo @ | Dye $FYEL)$1"
}

DisplayHeader()
{
  echo
  echo "$HEADER_SYSTEMINFO  $HEADER_PROJECTNAME  $HEADER_DATETIME"
  PrintFor $(($(echo "$HEADER_SYSTEMINFO" | Length) + 2)) ' '
  PrintFor $(echo "$HEADER_PROJECTNAME" | Length) '`'
  PrintFor $(($(echo "$HEADER_DATETIME" | Length) + 2)) ' '
}

DisplayFooter()
{
  echo
  printf "[ "
  PrintFor $(($(echo "$HEADER_SYSTEMINFO" | Length) - 2)) '='
  printf "  %s  " "$HEADER_PROJECTNAME"
  PrintFor $(($(echo "$HEADER_DATETIME" | Length) - 2)) '='
  printf " ]"
}

CollectFiles()
{
  files="";
  for item in "$1"/*; do
    if [ -f "$item" ]; then
      files="$files $item"
    fi
  done
  [ -z "$files" ] && NoFileCollected "$1"
  
  echo "$files"
}

PreInstall()
{
  AnnounceStage "PreInstall"
  
  cp -vr $(CollectFiles $preinstall_src) "$preinstall_dst/" 2>> "$ERROR" 1>> "$OUTPUT"
  _rtncode=$?
  
  $complain && Complain
  
  PrintStatus $_rtncode
  
  return $_rtncode
}

CompileObject()
{
  AnnounceStage "CompileObject"
  
  for source in $sources; do
    "$compiler" $flags -c "$source" -o "$outdir/$(basename $source).o" 2>> "$ERROR" 1>> "$OUTPUT" && output_files="$output_files $outdir/$(basename $source).o"
    _rtncode=$?
  
    cat "$OUTPUT" | Echo '  '
    
    test -n $_rtncode && break
  done
  
  $complain && Complain
  
  PrintStatus $_rtncode
  
  return $_rtncode
}

CompileExecutable()
{
  AnnounceStage "CompileExecutable"
  
  if [ -n "$link" ]; then
    "$compiler" $flags -L"$linkdir/"$link $sources -o "$bindir/$EXECUTABLE" 2>> "$ERROR" 1>> "$OUTPUT" && output_files="$output_files $bindir/$EXECUTABLE"
    _rtncode=$?
  else
    "$compiler" $flags $sources -o "$bindir/$EXECUTABLE" 2>> "$ERROR" 1>> "$OUTPUT" && output_files="$output_files $bindir/$EXECUTABLE"
    _rtncode=$?
  fi
  
  cat "$OUTPUT" | Echo '  '
  
  $complain && Complain
  
  PrintStatus $_rtncode
  
  return $_rtncode
}

CompileSharedObject()
{
  AnnounceStage "CompileSharedObject"

  if [ -n "$link" ]; then  
    "$compiler" $flags -L"$linkdir/"$link -shared $(CollectFiles "$outdir") -o "$bindir/$SHAREDOBJECT" 2>> "$ERROR" 1>> "$OUTPUT" && output_files="$output_files $bindir/$SHAREDOBJECT"
  else
    "$compiler" $flags -shared $(CollectFiles "$outdir") -o "$bindir/$SHAREDOBJECT" 2>> "$ERROR" 1>> "$OUTPUT" && output_files="$output_files $bindir/$SHAREDOBJECT"
  fi
  _rtncode=$?
  
  cat "$OUTPUT" | Echo '  '
  
  $complain && Complain
  
  PrintStatus $_rtncode
  
  return $_rtncode
}

PostInstall()
{
  AnnounceStage "PostInstall"
  
  cp -v "$postinstall_src"/"$SHAREDOBJECT" "$postinstall_dst" 2>> "$ERROR" 1>> "$OUTPUT"
  _rtncode=$?
  
  $complain && Complain
  
  PrintStatus $_rtncode
  
  return $_rtncode
}

Complain()
{
  cat "$OUTPUT" | ColorIndication | Dye $DIM | Echo '  '
  cat "$ERROR" | ColorIndication | Echo '  '
}

StartTimer()
{
  elapse_begin="$(date '+%s')"
  elapse_nano_begin="$(date '+%N')"
}

StopTimer()
{
  elapse_end="$(date '+%s')"
  elapse_nano_end="$(date '+%N')"
}

Elapse()
{
  elapse_totalseconds=$(($elapse_end - $elapse_begin))
  elapse_calcseconds=$(($elapse_totalseconds % 60))
  elapse_calcminutes=$(($elapse_totalseconds / 60 % 60))
  elapse_calchours=$(($elapse_totalseconds / 3600 % 60))
  elapse_calcmilliseconds=\
$(( ( $(RemoveLeadingZero $elapse_nano_end) + 1000000000 \
     - $(RemoveLeadingZero $elapse_nano_begin) ) / 1000000 % 1000 ))
  
  echo
  echo "  Elapsed:  $elapse_calchours$(echo h | Dye $DIM)"\
                   "$elapse_calcminutes$(echo m | Dye $DIM)"\
                   "$elapse_calcseconds$(echo s | Dye $DIM)"\
                   "$elapse_calcmilliseconds$(echo ms | Dye $DIM)"
}

ColorIndication()
{
  while IFS= read line; do
    case "$line" in
      *error:*)
        echo "$line" | Dye $FRED;;
        
      *warning:*)
        echo "$line" | Dye $FYEL;;
      
      *note:* |\
      *previously\ defined* |\
      *In\ file\ included* |\
      *from\ * |\
      *\ In\ function\ * |\
      *\ At\ top\ level:*)
        echo "$line" | Dye $FCYA ;;

      *)
        echo "$line" ;;
    esac
  done
}

Output()
{
  AnnounceStage "Output"
  
  count=1
  for item in $output_files; do
    echo "$count: $(echo $(dirname $item))/$(basename $item | Dye $FGRE)" | Echo '  '
    count=$((count + 1))
  done
}

DumpEnv()
{
  echo
  echo "      sources: $(echo $sources | Dye $DIM)"
  echo "     compiler: $(echo $compiler | Dye $DIM)"
  echo "        flags: $(echo $flags | Dye $DIM)"
  echo "       shared: $(echo $shared | Dye $DIM)"
  echo "       outdir: $(echo $outdir | Dye $DIM)"
  echo "       bindir: $(echo $bindir | Dye $DIM)"
  echo "      install: $(echo $install | Dye $DIM)"
  echo "   preinstall: $(echo $preinstall_src'  --> '$preinstall_dst | Dye $DIM)"
  echo "  postinstall: $(echo $postinstall_src'  --> '$postinstall_dst | Dye $DIM)"
  echo "     complain: $(echo $complain | Dye $DIM)"
  echo "         link: $(echo $link | Dye $DIM)"
  echo "      linkdir: $(echo $linkdir | Dye $DIM)"
  echo "       output: $(echo $output | Dye $DIM)"
  echo "        color: $(echo $color | Dye $DIM)"
}

CheckEnv()
{
  [ -n "$sources" ] && for source in $sources; do
    if [ ! -f "$source" ]; then
      FileDoesNotExist "$source"
    fi
  done
  
  if [ ! -f "$compiler" ]; then
    echo
    echo "  Checking failed on compiler." >&2
    FileDoesNotExist "$compiler"
  fi
  
  if [ ! -d "$outdir" ]; then
    echo
    echo "  Checking failed on outdir." >&2
    DirectoryDoesNotExist "$outdir"
  fi
  
  if [ ! -d "$bindir" ]; then
    echo
    echo "  Checking failed on bindir." >&2
    DirectoryDoesNotExist "$bindir"
  fi
  
  if $install && [ ! -d "$preinstall_src" ]; then
    echo
    echo "  Checking failed on preinstall_src." >&2
    DirectoryDoesNotExist "$preinstall_src"
  fi
  
  if $install && [ ! -d "$preinstall_dst" ]; then
    echo
    echo "  Checking failed on preinstall_dst." >&2
    DirectoryDoesNotExist "$preinstall_dst"
  fi
  
  if $install && [ ! -d "$postinstall_src" ]; then
    echo
    echo "  Checking failed on postinstall_src." >&2
    DirectoryDoesNotExist "$postinstall_src"
  fi
  
  if $install && [ ! -d "$postinstall_dst" ]; then
    echo
    echo "  Checking failed on postinstall_dst." >&2
    DirectoryDoesNotExist "$postinstall_dst"
  fi
  
  if [ ! -d "$linkdir" ]; then
    echo
    echo "  Checking failed on linkdir." >&2
    DirectoryDoesNotExist "$linkdir"
  fi
}

if [ $# -eq 0 ]; then
  PrintUsage
fi

while [ $# -gt 0 ]; do
  case "$1" in
    --sources)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      sources="$(echo "$1" | sed 's/,/ /g')" ;;

    --compiler)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      compiler="$1" ;;

    --flags)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      flags="$(echo "$1" | sed 's/,/ /g')" ;;

    --shared)
      shared=true ;;

    --outdir)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      outdir="$1" ;;

    --bindir)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      bindir="$1" ;;

    --install)
      install=true ;;

    --preinstall-src)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      preinstall_src="$1" ;;
      
    --preinstall-dst)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      preinstall_dst="$1" ;;
    
    --postinstall-src)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      postinstall_src="$1" ;;
      
    --postinstall-dst)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      postinstall_dst="$1" ;;

    --complain)
      complain=true ;;

    --link)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      link="$1" ;;

    --linkdir)
      [ $# -eq 1 ] && InsufficientArgument "$1"
      shift
      linkdir="$1" ;;

    --output)
      output=true ;;

    --dumpenv)
      dumpenv=true ;;

    --color)
      color=true ;;

    *)
      echo "Invalid flag '$1'"
      PrintUsage ;;
  esac
  
  shift
done

mkdir -p "$SCRIPT_DIRECTORY"
mkdir -p "$outdir"
mkdir -p "$bindir"
mkdir -p "$preinstall_dst"
mkdir -p "$postinstall_dst"

ResetFileBuffer
DisplayHeader

$dumpenv && DumpEnv
CheckEnv

StartTimer
$install && { PreInstall || RuntimeError; }
[ -n "$source" ] && { CompileObject || RuntimeError; }
if $shared; then
  [ -n "$source" ] && { CompileSharedObject || RuntimeError; }
else
  [ -n "$source" ] && { CompileExecutable || RuntimeError; }
fi
$install && $shared && { PostInstall || RuntimeError; }
StopTimer

$output && Output

Elapse
  
DisplayFooter

echo
HasError
