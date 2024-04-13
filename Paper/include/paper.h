#ifndef COMPOUND_PAPER_H
# define COMPOUND_PAPER_H

# include <Compound/common.h>
# include <Compound/pen.h>

typedef struct {
  wchar_t *buff;
  int bufflen;
  int lines;
  int columns;
  const int id;
} Paper;

Status    paper_create(Paper *paper, int lines, int columns);
Status    paper_constr(Paper *paper, wchar_t buff, int bufflen, int lines, 
                       int columns, const int id);
Status    paper_nilval(Paper *paper);
void      paper_delete(Paper *paper);

Selection paper_pen_select     (Paper *paper, Coordination start, Coordination end);
Selection paper_pen_selectby   (Paper *paper, int *target);
Status    paper_pen_copy       (Paper *paper, int **buff, Selection selc);
Status    paper_pen_erase      (Paper *paper, Selection selc);  // pen_write(pen_using, PEN_VALUE_EMPTY)
Status    paper_pen_erasefrom  (Paper *paper, Coordination coord);  // for (...) pen_writeat(pen_using, PEN_VALUE_EMPTY, { i, j })  // Erase from coord to EOF
Status    paper_pen_eraseto    (Paper *paper, Coordination coord);  // Erase from the beginning of file to coord
Status    paper_pen_find       (Paper *paper, wchar_t *target);  /* Select next token found by pen_find and return it.  Returns  */
Status    paper_pen_moveto     (Paper *paper, Coordination coord);
Status    paper_pen_paste      (Paper *paper, wchar_t *buff);
wchar_t   paper_pen_read       (Paper *paper);
Status    paper_pen_replace    (Paper *paper, wchar_t *target, wchar_t *replacement);
Status    paper_pen_selectfrom (Paper *paper, Coordination start);
Status    paper_pen_selectto   (Paper *paper, Coordination end);
Status    paper_pen_write      (Paper *paper, wchar_t value);

#endif  /* COMPOUND_PAPER_H */
