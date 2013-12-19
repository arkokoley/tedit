#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <string.h>
#include <conio.h>
#include <signal.h>
#define c 80
#define l 25

//#include <stat.h>
//#include <ioctl.h>
//#include <termios.h>
using namespace std;

#define NEW_LINE "\n"
#define MINEXTEND      32768
#define LINEBUF_EXTRA  32
#define TABSIZE        8

#define CLRSCR           "\033[0J"
#define CLREOL           "\033[K"
#define GOTOXY           "\033[%d;%dH"
#define RESET_COLOR      "\033[0m"

#ifdef COLOR
#define TEXT_COLOR       "\033[44m\033[37m\033[1m"
#define SELECT_COLOR     "\033[47m\033[37m\033[1m"
#define STATUS_COLOR     "\033[0m\033[47m\033[30m"
#else
#define TEXT_COLOR       "\033[0m"
#define SELECT_COLOR     "\033[7m\033[1m"
#define STATUS_COLOR     "\033[1m\033[7m"

#endif


#include "keys.h"

//
// Keyboard functions
//

void initkeys() {
	memset(last_keys, 0xFF, LAST_KEYS_LENGTH);
}

int getchar_logged() {
	int ch = getchar();
	for(int i = 1; i < LAST_KEYS_LENGTH; i++) {
		last_keys[i-1] = last_keys[i];
	}
	last_keys[LAST_KEYS_LENGTH - 1] = ch;
	return ch;
}

int getkey() {
	int ch, shift, ctrl;

	ch = getchar_logged();
	if (ch < 0)
		return ch;

	switch (ch) {
	case 0x08:
		return KEY_BACKSPACE;
	case 0x09:
		return KEY_TAB;
	case 0x0D:
		return KEY_ENTER;
	case 0x0A:
		return KEY_ENTER;
	case 0x1B:
		ch = getchar_logged();
		switch (ch) {
		case 0x1B:
			return KEY_ESC;
		case 0x4F:
			ch = getchar_logged();
			switch (ch) {
			case 0x46:
				return KEY_END;
			case 0x48:
				return KEY_HOME;
			case 0x50:
				return KEY_F1;
			case 0x52:
				return KEY_F3;
			case 0x54:
				return KEY_F5;
			default:
				return KEY_UNKNOWN;
			}
			break;

		case 0x5B:
			shift = ctrl = 0;
			ch = getchar_logged();
			if (ch == 0x31) {
				ch = getchar_logged();
				if (ch == 0x3B) {
					ch = getchar_logged();
					if (ch == 0x32)
						shift = 1;
					if (ch == 0x35)
						ctrl = 1;
					if (ch == 0x36)
						shift = ctrl = 1;
					ch = getchar_logged();
				} else {
					switch (ch) {
					case 0x35:
						return getchar_logged() == 0x7E ? KEY_F5 : KEY_UNKNOWN;
					case 0x37:
						return getchar_logged() == 0x7E ? KEY_F6 : KEY_UNKNOWN;
					case 0x38:
						return getchar_logged() == 0x7E ? KEY_F7 : KEY_UNKNOWN;
					}
				}
			}

			switch (ch) {
			case 0x31:
				return getchar_logged() == 0x7E ? KEY_HOME : KEY_UNKNOWN;
			case 0x32:
				return getchar_logged() == 0x7E ? KEY_INS : KEY_UNKNOWN;
			case 0x33:
				return getchar_logged() == 0x7E ? KEY_DEL : KEY_UNKNOWN;
			case 0x34:
				return getchar_logged() == 0x7E ? KEY_END : KEY_UNKNOWN;
			case 0x35:
				return getchar_logged() == 0x7E ? KEY_PGUP : KEY_UNKNOWN;
			case 0x36:
				return getchar_logged() == 0x7E ? KEY_PGDN : KEY_UNKNOWN;

			case 0x41:
				if (shift && ctrl)
					return KEY_SHIFT_CTRL_UP;
				if (shift)
					return KEY_SHIFT_UP;
				if (ctrl)
					return KEY_CTRL_UP;
				return KEY_UP;
			case 0x42:
				if (shift && ctrl)
					return KEY_SHIFT_CTRL_DOWN;
				if (shift)
					return KEY_SHIFT_DOWN;
				if (ctrl)
					return KEY_CTRL_DOWN;
				return KEY_DOWN;
			case 0x43:
				if (shift && ctrl)
					return KEY_SHIFT_CTRL_RIGHT;
				if (shift)
					return KEY_SHIFT_RIGHT;
				if (ctrl)
					return KEY_CTRL_RIGHT;
				return KEY_RIGHT;
			case 0x44:
				if (shift && ctrl)
					return KEY_SHIFT_CTRL_LEFT;
				if (shift)
					return KEY_SHIFT_LEFT;
				if (ctrl)
					return KEY_CTRL_LEFT;
				return KEY_LEFT;
			case 0x46:
				if (shift && ctrl)
					return KEY_SHIFT_CTRL_END;
				if (shift)
					return KEY_SHIFT_END;
				if (ctrl)
					return KEY_CTRL_END;
				return KEY_END;
			case 0x48:
				if (shift && ctrl)
					return KEY_SHIFT_CTRL_HOME;
				if (shift)
					return KEY_SHIFT_HOME;
				if (ctrl)
					return KEY_CTRL_HOME;
				return KEY_HOME;
			case 0x5A:
				return KEY_SHIFT_TAB;

			default:
				return KEY_UNKNOWN;
			}
			break;

		default:
			return KEY_UNKNOWN;
		}
		break;

	case 0x00:
	case 0xE0:
		ch = getchar_logged();
		switch (ch) {
		case 0x0F:
			return KEY_SHIFT_TAB;
		case 0x3B:
			return KEY_F1;
		case 0x3D:
			return KEY_F3;
		case 0x3F:
			return KEY_F5;
		case 0x47:
			return KEY_HOME;
		case 0x48:
			return KEY_UP;
		case 0x49:
			return KEY_PGUP;
		case 0x4B:
			return KEY_LEFT;
		case 0x4D:
			return KEY_RIGHT;
		case 0x4F:
			return KEY_END;
		case 0x50:
			return KEY_DOWN;
		case 0x51:
			return KEY_PGDN;
		case 0x52:
			return KEY_INS;
		case 0x53:
			return KEY_DEL;
		case 0x73:
			return KEY_CTRL_LEFT;
		case 0x74:
			return KEY_CTRL_RIGHT;
		case 0x75:
			return KEY_CTRL_END;
		case 0x77:
			return KEY_CTRL_HOME;
		case 0x8D:
			return KEY_CTRL_UP;
		case 0x91:
			return KEY_CTRL_DOWN;
		case 0x94:
			return KEY_CTRL_TAB;
		case 0xB8:
			return KEY_SHIFT_UP;
		case 0xB7:
			return KEY_SHIFT_HOME;
		case 0xBF:
			return KEY_SHIFT_END;
		case 0xB9:
			return KEY_SHIFT_PGUP;
		case 0xBB:
			return KEY_SHIFT_LEFT;
		case 0xBD:
			return KEY_SHIFT_RIGHT;
		case 0xC0:
			return KEY_SHIFT_DOWN;
		case 0xC1:
			return KEY_SHIFT_PGDN;
		case 0xDB:
			return KEY_SHIFT_CTRL_LEFT;
		case 0xDD:
			return KEY_SHIFT_CTRL_RIGHT;
		case 0xD8:
			return KEY_SHIFT_CTRL_UP;
		case 0xE0:
			return KEY_SHIFT_CTRL_DOWN;
		case 0xD7:
			return KEY_SHIFT_CTRL_HOME;
		case 0xDF:
			return KEY_SHIFT_CTRL_END;

		default:
			return KEY_UNKNOWN;
		}
		break;

	case 0x7F:
		return KEY_BACKSPACE;

	default:
		return ch;
	}
}

//
// Editor data block
//
// Structure of split buffer:
//
//    +------------------+------------------+------------------+
//    | text before gap  |        gap       |  text after gap  |
//    +------------------+------------------+------------------+
//    ^                  ^                  ^                  ^
//    |                  |                  |                  |
//  start               gap                rest               end
//

struct editor {
	 char *start; // Start of text buffer
	 char *gap; // Start of gap
	 char *rest; // End of gap
	 char *end; // End of text buffer

	int toppos; // Text position for current top screen line
	int topline; // Line number for top of screen
	int margin; // Position for leftmost column on screen

	int linepos; // Text position for current line
	int line; // Current document line
	int col; // Current document column
	int lastcol; // Remembered column from last horizontal navigation
	int anchor; // Anchor position for selection

	struct undo *undohead; // Start of undo buffer list
	struct undo *undotail; // End of undo buffer list
	struct undo *undo; // Undo/redo boundary

	int refresh; // Flag to trigger screen redraw
	int lineupdate; // Flag to trigger redraw of current line
	int dirty; // Dirty flag is set when the editor buffer has been changed

	int newfile; // File is a new file

	struct env *env; // Reference global editor environment
	struct editor *next; // Next editor.
	struct editor *prev; // Previous editor

	char filename[80];
	void delete_editor();
	int new_file(char *filename);
	int load_file(char *file);
	int save_file();
	int text_length();
	char* text_ptr(int);
	void move_gap(int,int);
	void close_gap();
	int get(int);
	int copy(char*, int,int);
	void replace(int,int,char*,int,int);
	void insert(int,char*, int);
	void erase_section(int,int);
	int line_length(int);
	int next_line(int);
	int prev_line(int);
	int column(int,int);
	void moveto(int, int);
	int get_selection(int*, int*);
	int get_selected_text(char*, int);
	int erase_selection();
	void update_selection(int);
	int line_start(int);
	void select_all();
	int prompt(char*);
	void display_message(char*);
	void draw_full_statusline();
};

struct env {
	struct editor *current; // Current editor

	char *clipboard; // Clipboard
	int clipsize; // Clipboard size

	char *search; // Search text.

	char *linebuf; // Scratch buffer.

	int cols; // Console columns
	int lines; // Console lines

	int untitled; // Counter for untitled files
};

//
// Editor buffer functions
//

struct editor *create_editor(struct env *env) {
  struct editor *ed = (struct editor *) malloc(sizeof(struct editor));
  memset(ed, 0, sizeof(struct editor));
  if (env->current) {
    ed->next = env->current->next;
    ed->prev = env->current;
    env->current->next->prev = ed;
    env->current->next = ed;
  } else {
    ed->next = ed->prev = ed;
  }
  ed->env = env;
  env->current = ed;
  return ed;
}

void delete_editor(struct editor *ed) {
  if (ed->next == ed) {
    ed->env->current = NULL;
  } else {
    ed->env->current = ed->prev;
  }
  ed->next->prev = ed->prev;
  ed->prev->next = ed->next;
  if (ed->start) free(ed->start);
  free(ed);
}

struct editor *find_editor(struct env *env, char *filename) {
  char fn[FILENAME_MAX];
  struct editor *ed = env->current;
  struct editor *start = ed;
  
  if (!realpath(filename, fn)) strcpy(fn, filename);

  do {
    if (strcmp(fn, ed->filename) == 0) return ed;
    ed = ed->next;
  } while (ed != start);
  return NULL;  
}
int editor::new_file(char *file) {
	if (*filename) {
		strcpy(filename, file);
	} else {
		cout<<filename<<"Untitled-"<<++env->untitled;
		newfile = 1;
	}

	start = ( char *) malloc(MINEXTEND);
	if (!start)
		return -1;
#ifdef DEBUG
	memset(start, 0, MINEXTEND);
#endif

	gap = start;
	rest = end = gap + MINEXTEND;
	anchor = -1;

	return 0;
}

int editor::load_file(char *file) {
	struct stat statbuf;
	int length;

	//if (!realpath(filename, filename))
	//	return -1;
	ifstream f(filename);
	if (!f)
		return -1;

	//if (fstat(f, &statbuf) < 0)
	//	goto err;
	length = statbuf.st_size;

	start = ( char *) malloc(length + MINEXTEND);
	if (!start)
		goto err;
#ifdef DEBUG
	memset(start, 0, length + MINEXTEND);
#endif
//	if (f.read(start, length) != length)
//		goto err;

	gap = start + length;
	rest = end = gap + MINEXTEND;
	anchor = -1;

	f.close();
	return 0;

	err: f.close();
	if (start) {
		free(start);
		start = NULL;
	}
	return -1;
}

int editor::save_file() {
	fstream f;

	f.open(filename);
	if (f < 0)
		return -1;
	char *ch=start;
	while(ch!=gap) {
	f<<start;
	ch++;
	}
	ch=rest;
	while(ch!=end) {
	f<<rest;
	ch++;
	}
	f.close();
	dirty = 0;
	return 0;

}

int editor::text_length() {
	return (gap - start) + (end - rest);
}

 char *editor::text_ptr(int pos) {
	 char *p = start + pos;
	if (p >= gap)
		p += (rest - gap);
	return p;
}

void editor::move_gap(int pos, int minsize) {
	int gapsize = rest - gap;
	 char *p = text_ptr(pos);
	if (minsize < 0)
		minsize = 0;

	if (minsize <= gapsize) {
		if (p != rest) {
			if (p < gap) {
				memmove(p + gapsize, p, gap - p);
			} else {
				memmove(gap, rest, p - rest);
			}
			gap = start + pos;
			rest = gap + gapsize;
		}
	} else {
		int newsize;
		 char *newstart;
		 char *newgap;
		 char *newrest;
		 char *newend;

		if (gapsize + MINEXTEND > minsize)
			minsize = gapsize + MINEXTEND;
		newsize = (end - start) - gapsize + minsize;
		start = ( char *) malloc(newsize); // TODO check for out of memory
		gap = start + pos;
		rest = gap + minsize;
		end = start + newsize;

		if (p < gap) {
			memcpy(newstart, start, pos);
			memcpy(newrest, p, gap - p);
			memcpy(newend - (end - rest), rest, end - rest);
		} else {
			memcpy(newstart, start, gap - start);
			memcpy(newstart + (gap - start), rest, p - rest);
			memcpy(newrest, p, end - p);
		}

		free(start);
		start = newstart;
		gap = newgap;
		rest = newrest;
		end = newend;
	}

#ifdef DEBUG
	memset(gap, 0, rest - gap);
#endif
}

void editor::close_gap() {
	int len = text_length();
	move_gap( len, 1);
	start[len] = 0;
}

int editor::get( int pos) {
	 char *p = text_ptr( pos);
	if (p >= end)
		return -1;
	return *p;
}

int editor::copy(char *buf, int pos, int len) {
	 char *bufptr = buf;
	 char *p = start + pos;
	if (p >= gap)
		p += (rest - gap);

	while (len > 0) {
		if (p == end)
			break;
		*bufptr++ = *p;
		len--;
		if (++p == gap)
			p = rest;
	}

	return bufptr - buf;
}

void editor::replace(int pos, int len,  char *buf, int bufsize, int doundo) {
	 char *p = start + pos;

	if (bufsize == 0 && p <= gap && p + len >= gap) {
		// Handle deletions at the edges of the gap
		rest += len - (gap - p);
		gap = p;
	} else {
		// Move the gap
		move_gap(pos + len, bufsize - len);

		// Replace contents
		memcpy(start + pos, buf, bufsize);
		gap = start + pos + bufsize;
	}

	// Mark buffer as dirty
	dirty = 1;
}

void editor::insert( int pos,  char *buf, int bufsize) {
	replace(pos, 0, buf, bufsize, 1);
}

void editor::erase_section( int pos, int len) {
	replace( pos, len, NULL, 0, 1);
}

//
// Navigation functions
//

int editor::line_length( int linepos) {
	int pos = linepos;
	while (1) {
		int ch = get( pos);
		if (ch < 0 || ch == '\n' || ch == '\r')
			break;
		pos++;
	}

	return pos - linepos;
}

int editor::line_start( int pos) {
	while (1) {
		if (pos == 0)
			break;
		if (get(pos - 1) == '\n')
			break;
		pos--;
	}

	return pos;
}

int editor::next_line(int pos) {
	while (1) {
		int ch = get( pos);
		if (ch < 0)
			return -1;
		pos++;
		if (ch == '\n')
			return pos;
	}
}

int editor::prev_line( int pos) {
	if (pos == 0)
		return -1;

	while (pos > 0) {
		int ch = get(--pos);
		if (ch == '\n')
			break;
	}

	while (pos > 0) {
		int ch = get(--pos);
		if (ch == '\n')
			return pos + 1;
	}

	return 0;
}

int editor::column(int linepos, int col) {
	 char *p = text_ptr( linepos);
	int co = 0;
	while (col > 0) {
		if (p == end)
			break;
		if (*p == '\t') {
			int spaces = TABSIZE - co % TABSIZE;
			co += spaces;
		} else {
			co++;
		}
		col--;
		if (++p == gap)
			p = rest;
	}
	return co;
}

void editor::moveto(int pos, int center) {
	int scroll = 0;
	for (;;) {
		int cur = linepos + col;
		if (pos < cur) {
			if (pos >= linepos) {
				col = pos - linepos;
			} else {
				col = 0;
				linepos = prev_line(linepos);
				line--;

				if (topline > line) {
					toppos = linepos;
					topline--;
					refresh = 1;
					scroll = 1;
				}
			}
		} else if (pos > cur) {
			int next = next_line(linepos);
			if (next == -1) {
				col = text_length() - linepos;
				break;
			} else if (pos < next) {
				col = pos - linepos;
			} else {
				col = 0;
				linepos = next;
				line++;

				if (line >= topline + env->lines) {
					toppos = next_line(toppos);
					topline++;
					refresh = 1;
					scroll = 1;
				}
			}
		} else {
			break;
		}
	}

	if (scroll && center) {
		int tl = line -= env->lines / 2;
		if (tl < 0)
			tl = 0;
		for (;;) {
			if (topline > tl) {
				toppos = prev_line(toppos);
				topline--;
			} else if (topline < tl) {
				toppos = next_line(toppos);
				topline++;
			} else {
				break;
			}
		}
	}
}

//
// Text selection
//

int editor::get_selection(int *start, int *end) {
	if (anchor == -1) {
		*start = *end = -1;
		return 0;
	} else {
		int pos = linepos + col;
		if (pos == anchor) {
			*start = *end = -1;
			return 0;
		} else if (pos < anchor) {
			*start = pos;
			*end = anchor;
		} else {
			*start = anchor;
			*end = pos;
		}
	}
	return 1;
}

int editor::get_selected_text(char *buffer, int size) {
	int selstart, selend, len;

	if (!get_selection(&selstart, &selend))
		return 0;
	len = selend - selstart;
	if (len >= size)
		return 0;
	copy(buffer, selstart, len);
	buffer[len] = 0;
	return len;
}

void editor::update_selection(int select) {
	if (select) {
		if (anchor == -1)
			anchor = linepos + col;
		refresh = 1;
	} else {
		if (anchor != -1)
			refresh = 1;
		anchor = -1;
	}
}

int editor::erase_selection() {
	int selstart, selend;

	if (!get_selection( &selstart, &selend))
		return 0;
	moveto(selstart, 0);
	erase_section(selstart, selend - selstart);
	anchor = -1;
	refresh = 1;
	return 1;
}

void editor::select_all() {
	anchor = 0;
	refresh = 1;
	moveto(text_length(), 0);
}


//
// Screen functions
//

void get_console_size(struct env *env) {


	//ioctl(0, TIOCGWINSZ, &ws);
	env->cols = c; //ws.ws_col;
	env->lines = l-1; //ws.ws_row - 1;

//	env->linebuf = realloc(void , env->cols + LINEBUF_EXTRA);
}

void outch(char ch) {
	putchar(ch);
}

void outbuf(char *buf, int len) {
	fwrite(buf, 1, len, stdout);
}

void outstr(char *str) {
	fputs(str, stdout);
}

void clear_screen() {
	outstr(CLRSCR);
}

void gotoxy(int col, int line) {
	char buf[32];

	sprintf(buf, GOTOXY, line + 1, col + 1);
	outstr(buf);
}

int editor::prompt( char *msg) {
	int maxlen, len, ch;
	char *buf = env->linebuf;

	gotoxy(0, env->lines);
	outstr(STATUS_COLOR);
	outstr(msg);
	outstr(CLREOL);

	len = 0;
	maxlen = env->cols - strlen(msg) - 1;
	len = get_selected_text( buf, maxlen);
	outbuf(buf, len);

	for (;;) {
		fflush(stdout);
		ch = getkey();
		if (ch == KEY_ESC) {
			return 0;
		} else if (ch == KEY_ENTER) {
			buf[len] = 0;
			return len > 0;
		} else if (ch == KEY_BACKSPACE) {
			if (len > 0) {
				outstr("\b \b");
				len--;
			}
		} else if (ch >= ' ' && ch < 0x100 && len < maxlen) {
			outch(ch);
			buf[len++] = ch;
		}
	}
}

int ask() {
	int ch = getchar();
	return ch == 'y' || ch == 'Y';
}

//
// Display functions
//

void editor::display_message(char *fmt, ...) {
	va_list args;

	va_start(args, fmt);
	gotoxy(0, env->lines);
	outstr(STATUS_COLOR);
	vprintf(fmt, args);
	outstr(CLREOL TEXT_COLOR);
	fflush(stdout);
	va_end(args);
}

void editor::draw_full_statusline() {
	struct env *env = this->env;
	int namewidth = env->cols - 19;

	gotoxy(0, env->lines);
	sprintf(env->linebuf,
			STATUS_COLOR "%*.*s%c Ln %-6dCol %-4d" CLREOL TEXT_COLOR,
			-namewidth, namewidth, filename, dirty ? '*' : ' ',
			line + 1, column(linepos, col) + 1);
	outstr(env->linebuf);
#ifdef DEBUG
	gotoxy(0, env->lines - 1);
	sprintf(env->linebuf,
			STATUS_COLOR "[%02X%02X%02X%02X%02X%02X]" TEXT_COLOR,
			last_keys[0], last_keys[1], last_keys[2], last_keys[3], last_keys[4],
			last_keys[5]);
	outstr(env->linebuf);
#endif
}

void editor::display_line(int pos, int fullline) {
	int hilite = 0;
	int col = 0;
	int margin = margin;
	int maxcol = env->cols + margin;
	char *bufptr = env->linebuf;
	 char *p = text_ptr( pos);
	int selstart, selend, ch;
	char *s;

	get_selection(&selstart, &selend);
	while (col < maxcol) {
			if (margin == 0) {
		if (!hilite && pos >= selstart && pos < selend) {
				for (s = SELECT_COLOR; *s; s++)
					*bufptr++ = *s;
				hilite = 1;
			} else if (hilite && pos >= selend) {
				for (s = TEXT_COLOR; *s; s++)
					*bufptr++ = *s;
				hilite = 0;
			}
		}
		if (p == end)
			break;
		ch = *p;
		if (ch == '\r' || ch == '\n')
			break;

		if (ch == '\t') {
			int spaces = TABSIZE - col % TABSIZE;
			while (spaces > 0 && col < maxcol) {
				if (margin > 0) {
					margin--;
				} else {
					*bufptr++ = ' ';
				}
				col++;
				spaces--;
			}
		} else {
			if (margin > 0) {
				margin--;
			} else {
				*bufptr++ = ch;
			}
			col++;
		}

		if (++p == gap)
			p = rest;
		pos++;
	}

	if (hilite) {
		while (col < maxcol) {
			*bufptr++ = ' ';
			col++;
		}
	} else {
		if (col == margin)
			*bufptr++ = ' ';
	}

	if (col < maxcol) {
		for (s = CLREOL; *s; s++)
			*bufptr++ = *s;
		if (fullline) {
			memcpy(bufptr, "\r\n", 2);
			bufptr += 2;
		}
	}

	if (hilite) {
		for (s = TEXT_COLOR; *s; s++)
			*bufptr++ = *s;
	}

	outbuf(env->linebuf, bufptr - env->linebuf);
}

void editor::update_line() {
	gotoxy(0, line - topline);
	display_line(linepos, 0);
}

void editor::draw_screen() {
	int pos;
	int i;

	gotoxy(0, 0);
	outstr(TEXT_COLOR);
	pos = toppos;
	for (i = 0; i < env->lines; i++) {
		if (pos < 0) {
			outstr(CLREOL "\r\n");
		} else {
			display_line( pos, 1);
			pos = next_line( pos);
		}
	}
}

void editor::position_cursor() {
	int col = column(linepos, col);
	gotoxy(col - margin, line - topline);
}

//
// Cursor movement
//

void editor::adjust() {
	int col;
	int ll = line_length(linepos);
	col = lastcol;
	if (col > ll)
		col = ll;

	col = column(linepos, col);
	while (col < margin) {
		margin -= 4;
		if (margin < 0)
			margin = 0;
		refresh = 1;
	}

	while (col - margin >= env->cols) {
		margin += 4;
		refresh = 1;
	}
}

void editor::up( int select) {
	int newpos = prev_line(linepos);
	if (newpos < 0)
		return;

	update_selection( select);

	linepos = newpos;
	line--;
	if (line < topline) {
		toppos = linepos;
		topline = line;
		refresh = 1;
	}

	adjust();
}

void editor::down( int select) {
	int newpos = next_line(linepos);
	if (newpos < 0)
		return;

	update_selection(select);

	linepos = newpos;
	line++;

	if (line >= topline + env->lines) {
		toppos = next_line(toppos);
		topline++;
		refresh = 1;
	}

	adjust();
}

void editor::left( int select) {
	update_selection( select);
	if (col > 0) {
		col--;
	} else {
		int newpos = prev_line(linepos);
		if (newpos < 0)
			return;

		col = line_length(newpos);
		linepos = newpos;
		line--;
		if (line < topline) {
			toppos = linepos;
			topline = line;
			refresh = 1;
		}
	}

	lastcol = col;
	adjust();
}

void editor::right( int select) {
	update_selection(select);
	if (col < line_length(linepos)) {
		col++;
	} else {
		int newpos = next_line(linepos);
		if (newpos < 0)
			return;

		col = 0;
		linepos = newpos;
		line++;

		if (line >= topline + env->lines) {
			toppos = next_line(toppos);
			topline++;
			refresh = 1;
		}
	}

	lastcol =col;
	adjust();
}

int wordchar(int ch) {
	return (ch >= 'A' && ch <= 'Z')
		|| (ch >= 'a' && ch <= 'z')
		|| (ch >= '0' && ch <= '9');
}

void editor::wordleft( int select) {
	int pos, phase;

	update_selection(select);
	pos = linepos + col;
	phase = 0;
	while (pos > 0) {
		int ch = get(pos - 1);
		if (phase == 0) {
			if (wordchar(ch))
				phase = 1;
		} else {
			if (!wordchar(ch))
				break;
		}

		pos--;
		if (pos < linepos) {
			linepos = prev_line(linepos);
			line--;
			refresh = 1;
		}
	}
	col = pos - linepos;
	if (line < topline) {
		toppos = linepos;
		topline = line;
	}

	lastcol = col;
	adjust();
}

void editor::wordright(int select) {
	int pos, end, phase, next;

	update_selection(select);
	pos = linepos + col;
	end = text_length(ed);
	next = next_line(ed, linepos);
	phase = 0;
	while (pos < end) {
		int ch = get(ed, pos);
		if (phase == 0) {
			if (wordchar(ch))
				phase = 1;
		} else {
			if (!wordchar(ch))
				break;
		}

		pos++;
		if (pos == next) {
			linepos = next;
			next = next_line(linepos);
			line++;
			refresh = 1;
		}
	}
	col = pos - linepos;
	if (line >= topline + env->lines) {
		toppos = next_line(toppos);
		topline++;
	}

	lastcol = col;
	adjust();
}

void editor::home( int select) {
	update_selection( select);
	col = lastcol = 0;
	adjust();
}

void editor::endkey( int select) {
	update_selection( select);
	col = lastcol = line_length(linepos);
	adjust();
}

void editor::top( int select) {
	update_selection( select);
	toppos = topline = margin = 0;
	linepos = line = col = lastcol = 0;
	refresh = 1;
}

void editor::bottom( int select) {
	update_selection(select);
	for (;;) {
		int newpos = next_line(linepos);
		if (newpos < 0)
			break;

		linepos = newpos;
		line++;

		if (line >= topline + env->lines) {
			toppos = next_line(toppos);
			topline++;
			refresh = 1;
		}
	}
	col = lastcol = line_length(linepos);
	adjust();
}

void editor::pageup( int select) {
	int i;

	update_selection(select);
	if (line < env->lines) {
		linepos = toppos = 0;
		line = topline = 0;
	} else {
		for (i = 0; i < env->lines; i++) {
			int newpos = prev_line(ed, linepos);
			if (newpos < 0)
				return;

			linepos = newpos;
			line--;

			if (topline > 0) {
				toppos = prev_line(ed, toppos);
				topline--;
			}
		}
	}

	refresh = 1;
	adjust();
}

void editor::pagedown(int select) {
	int i;

	update_selection( select);
	for (i = 0; i < env->lines; i++) {
		int newpos = next_line(linepos);
		if (newpos < 0)
			break;

		linepos = newpos;
		line++;

		toppos = next_line( toppos);
		topline++;
	}

	refresh = 1;
	adjust();
}

//
// Text editing
//

void editor::insert_char( char ch) {
	erase_selection();
	insert(linepos + col, &ch, 1);
	col++;
	lastcol = col;
	adjust();
	if (!refresh)
		lineupdate = 1;
}

void editor::newline() {
	int p;
	 char ch;

	erase_selection();
	insert(linepos + col, NEW_LINE, 1);
	col = lastcol = 0;
	line++;
	p = linepos;
	linepos = next_line(linepos);

	// This causes whitespace to be copied over to new lines.
	// Causes issues if pasting text in, so disabled for now.
	/*
	for (;;) {
		ch = get(ed, p++);
		if (ch == ' ' || ch == '\t') {
			insert(ed, linepos + col, &ch, 1);
			col++;
		} else {
			break;
		}
	}
	*/

	lastcol = col;

	refresh = 1;

	if (line >= topline + env->lines) {
		toppos = next_line(toppos);
		topline++;
		refresh = 1;
	}

	adjust();
}

void editor::backspace() {
	if (erase_selection())
		return;
	if (linepos + col == 0)
		return;
	if (col == 0) {
		int pos = linepos;
		erase_section(--pos, 1);
		if (get( pos - 1) == '\r')
			erase_section( --pos, 1);

		line--;
		linepos = line_start( pos);
		col = pos - linepos;
		refresh = 1;

		if (line < topline) {
			toppos = linepos;
			topline = line;
		}
	} else {
		col--;
		erase_section( linepos + col, 1);
		lineupdate = 1;
	}

	lastcol = col;
	adjust();
}

void editor::del() {
	int pos, ch;

	if (erase_selection())
		return;
	pos = linepos + col;
	ch = get( pos);
	if (ch < 0)
		return;

	erase_section(pos, 1);
	if (ch == '\r') {
		ch = get(ed, pos);
		if (ch == '\n')
			erase_section( pos, 1);
	}

	if (ch == '\n') {
		refresh = 1;
	} else {
		lineupdate = 1;
	}
}


//
// Clipboard
//

void editor::copy_selection() {
	int selstart, selend;

	if (!get_selection( &selstart, &selend))
		return;
	env->clipsize = selend - selstart;
	//env->clipboard = realloc(env->clipboard,	env->clipsize);
	copy( env->clipboard, selstart, env->clipsize);
}

void editor::cut_selection() {
	copy_selection();
	erase_selection();
}

void editor::paste_selection() {
	erase_selection();
	insert(linepos + col, env->clipboard, env->clipsize);
	moveto(linepos + col + env->clipsize, 0);
	refresh = 1;
}

//
// Editor Commands
//

void open_editor(struct editor *ed) {
  int rc;
  char *filename;
  struct env *env = ed->env;

  if (!prompt(ed, "Open file: ")) {
    ed->refresh = 1;
    return;
  }
  filename = ed->env->linebuf;
  
  ed = find_editor(ed->env, filename);
  if (ed) {
    env->current = ed;
  } else {
    ed = create_editor(env);
    rc = load_file(ed, filename);
    if (rc < 0) {
      display_message(ed, "Error %d opening %s (%s)", errno, filename, strerror(errno));
      sleep(5);
      delete_editor(ed);
      ed = env->current;
    }
  }
  ed->refresh = 1;
}

void new_editor(struct editor *ed) {
  ed = create_editor(ed->env);
  new_file(ed, "");
  ed->refresh = 1;
}

void read_from_stdin(struct editor *ed) {
  char buffer[512];
  int n, pos;

  pos = 0;
  while ((n = fread(buffer, 1, sizeof(buffer), stdin)) > 0) {
    insert(ed, pos, buffer, n);
    pos += n;
  }
  strcpy(ed->filename, "<stdin>");  
  ed->newfile = 1;
  ed->dirty = 0;
}

void save_editor(struct editor *ed) {
  int rc;
  
  if (!ed->dirty && !ed->newfile) return;
  
  if (ed->newfile) {
    if (!prompt(ed, "Save as: ")) {
      ed->refresh = 1;
      return;
    }

    if (access(ed->env->linebuf, F_OK) == 0) {
      display_message(ed, "Overwrite %s (y/n)? ", ed->env->linebuf);
      if (!ask()) {
        ed->refresh = 1;
        return;
      }
    }
    strcpy(ed->filename, ed->env->linebuf);
    ed->newfile = 0;
  }

  rc = save_file(ed);
  if (rc < 0) {
    display_message(ed, "Error %d saving document (%s)", errno, strerror(errno));
    sleep(5);
  }

  ed->refresh = 1;
}

void close_editor(struct editor *ed) {
  struct env *env = ed->env;
  
  if (ed->dirty) {
    display_message(ed, "Close %s without saving changes (y/n)? ", ed->filename);
    if (!ask()) {
      ed->refresh = 1;
      return;
    }
  }
  
  delete_editor(ed);

  ed = env->current;
  if (!ed) {
    ed = create_editor(env);
    new_file(ed, "");
  }
  ed->refresh = 1;
}

void editor::find_text(int next) {
	int slen;

	if (!next) {
		if (!prompt("Find: ")) {
			refresh = 1;
			return;
		}
		if (env->search)
			free(env->search);
		env->search = strdup(env->linebuf);
	}

	if (!env->search)
		return;
	slen = strlen(env->search);
	if (slen > 0) {
		 char *match;

		close_gap();
		match = strstr(start + linepos + col, env->search);
		if (match != NULL) {
			int pos = match - start;
			anchor = pos;
			moveto(pos + slen, 1);
		} else {
			outch('\007');
		}
	}
	refresh = 1;
}

void editor::goto_line() {
	int lineno, li, pos;

	anchor = -1;
	if (prompt("Goto line: ")) {
		lineno = atoi(env->linebuf);
		if (lineno > 0) {
			pos = 0;
			for (li = 0; li < lineno - 1; li++) {
				pos = next_line(pos);
				if (pos < 0)
					break;
			}
		} else {
			pos = -1;
		}

		if (pos >= 0) {
			moveto(pos, 1);
		} else {
			outch('\007');
		}
	}
	refresh = 1;
}

struct editor *next_file(struct editor *ed) {
	ed = env->current = next;
	refresh = 1;
	return ed;
}

struct editor *prev_file(struct editor *ed) {
	ed = env->current = prev;
	refresh = 1;
	return ed;
}

void jump_to_editor(struct editor *ed) {
  struct env *env = ed->env;
  char filename[FILENAME_MAX];
  int lineno = 0;

  if (!get_selected_text(ed, filename, FILENAME_MAX)) {
    int pos = ed->linepos + ed->col;
    char *p = filename;
    int left = FILENAME_MAX - 1;
    while (left > 0) {
      int ch = get(ed, pos);
      if (ch < 0) break;
      if (strchr("!@\"'#%&()[]{}*?+:;\r\n\t ", ch)) break;
      *p++ = ch;
      left--;
      pos++;
    }
    *p = 0;

    if (get(ed, pos) == ':') {
      pos++;
      for (;;) {
        int ch = get(ed, pos);
        if (ch < 0) break;
        if (ch >= '0' && ch <= '9') {
          lineno = lineno * 10 + (ch - '0');
        } else {
          break;
        }
        pos++;
      }
    }
  }
  if (!*filename) return;
  
  ed = find_editor(env, filename);
  if (ed) {
    env->current = ed;
  } else {
    ed = create_editor(env);
    if (load_file(ed, filename) < 0) {
      outch('\007');
      delete_editor(ed);
      ed = env->current;
    }
  }
  
  if (lineno > 0) {
    int pos = 0;
    while (--lineno > 0) {
      pos = next_line(ed, pos);
      if (pos < 0) break;
    }
    if (pos >= 0) moveto(ed, pos, 1);
  }

  ed->refresh = 1;
}

void redraw_screen(struct editor *ed) {
  get_console_size(ed->env);
  draw_screen(ed);
}

int quit(struct env *env) {
  struct editor *ed = env->current;
  struct editor *start = ed;

  do {
    if (ed->dirty) {
      display_message(ed, "Close %s without saving changes (y/n)? ", ed->filename);
      if (!ask()) return 0;
    }
    ed = ed->next;
  } while (ed != start);

  return 1;
}

void help(struct editor *ed) {
	gotoxy(0, 0);
	clear_screen();
	outstr("Editor Command Summary\r\n");
	outstr("======================\r\n\r\n");
	outstr("<up>         Move one line up (*)         Ctrl+N  New editor\r\n");
	outstr("<down>       Move one line down (*)       Ctrl+O  Open file\r\n");
	outstr("<left>       Move one character left (*)  Ctrl+S  Save file\r\n");
	outstr("<right>      Move one character right (*) Ctrl+W  Close file\r\n");
	outstr("<pgup>       Move one page up (*)         Ctrl+Q  Quit\r\n");
	outstr(
			"<pgdn>       Move one page down (*)       Ctrl+P  Pipe command\r\n");
	outstr("Ctrl+<left>  Move to previous word (*)    Ctrl+A  Select all\r\n");
	outstr(
			"Ctrl+<right> Move to next word (*)        Ctrl+C  Copy selection to clipboard\r\n");
	outstr(
			"<home>       Move to start of line (*)    Ctrl+X  Cut selection to clipboard\r\n");
	outstr(
			"<end>        Move to end of line (*)      Ctrl+V  Paste from clipboard\r\n");
	outstr("Ctrl+<home>  Move to start of file (*)    Ctrl+Z  Undo\r\n");
	outstr("Ctrl+<end>   Move to end of file (*)      Ctrl+R  Redo\r\n");
	outstr("<backspace>  Delete previous character    Ctrl+F  Find text\r\n");
	outstr("<delete>     Delete current character     Ctrl+G  Find next\r\n");
	outstr("Shift+<tab>  Next editor                  Ctrl+L  Goto line\r\n");
	outstr("Ctrl+<tab>   Previous editor              F1      Help\r\n");
	outstr(
			"                                          F3      Navigate to file\r\n");
	outstr(
			"(*) Extends selection if combined         F5      Redraw screen\r\n");
	outstr("    with Shift\r\n");
	outstr("\r\nPress any key to continue...");
	fflush(stdout);

	getkey();
	draw_screen(ed);
//	draw_full_statusline(ed);
}

//
// Editor
//

void editor::edit() {
	int done = 0;
	int key;

	refresh = 1;
	while (!done) {
		if (refresh) {
			draw_screen();
			draw_full_statusline();
			refresh = 0;
			lineupdate = 0;
		} else if (lineupdate) {
			update_line();
			lineupdate = 0;
			draw_full_statusline();
		} else {
			draw_full_statusline();
		}

		position_cursor();
		fflush(stdout);
		key = getkey();

		if (key >= ' ' && key <= 0x7F) {
#ifndef LESS
			insert_char(( char) key);
#endif
		} else {
			switch (key) {
			case KEY_F1:
				help();
				break;
			case KEY_F3:
				jump_to_editor();
				this = env->current;
				break;
			case KEY_F5:
				redraw_screen();
				break;
			case ctrl('u'):
				jump_to_editor();
				this = env->current;
				break;
			case ctrl('y'):
				help();
				break;
			case ctrl('t'):
				top(0);
				break;
			case ctrl('b'):
				bottom(0);
				break;
			case KEY_UP:
				up( 0);
				break;
			case KEY_DOWN:
				down( 0);
				break;
			case KEY_LEFT:
				left( 0);
				break;
			case KEY_RIGHT:
				right( 0);
				break;
			case KEY_HOME:
				home( 0);
				break;
			case KEY_END:
				end( 0);
				break;
			case KEY_PGUP:
				pageup( 0);
				break;
			case KEY_PGDN:
				pagedown( 0);
				break;

			case KEY_CTRL_RIGHT:
				wordright( 0);
				break;
			case KEY_CTRL_LEFT:
				wordleft( 0);
				break;
			case KEY_CTRL_HOME:
				top( 0);
				break;
			case KEY_CTRL_END:
				bottom( 0);
				break;

			case KEY_SHIFT_UP:
				up( 1);
				break;
			case KEY_SHIFT_DOWN:
				down( 1);
				break;
			case KEY_SHIFT_LEFT:
				left( 1);
				break;
			case KEY_SHIFT_RIGHT:
				right( 1);
				break;
			case KEY_SHIFT_PGUP:
				pageup( 1);
				break;
			case KEY_SHIFT_PGDN:
				pagedown( 1);
				break;
			case KEY_SHIFT_HOME:
				home( 1);
				break;
			case KEY_SHIFT_END:
				end( 1);
				break;

			case KEY_SHIFT_CTRL_RIGHT:
				wordright( 1);
				break;
			case KEY_SHIFT_CTRL_LEFT:
				wordleft( 1);
				break;
			case KEY_SHIFT_CTRL_HOME:
				top( 1);
				break;
			case KEY_SHIFT_CTRL_END:
				bottom( 1);
				break;

			case KEY_SHIFT_TAB:
				this = next_file(this);
				break;
			case KEY_CTRL_TAB:
				this = prev_file(this);
				break;

			case ctrl('a'):
				select_all();
				break;
			case ctrl('c'):
				copy_selection();
				break;
			case ctrl('f'):
				find_text( 0);
				break;
			case ctrl('l'):
				goto_line();
				break;
			case ctrl('g'):
				find_text( 1);
				break;
			case ctrl('q'):
				done = 1;
				break;
#ifdef LESS
				case KEY_ESC: done = 1; break;
#else
			case KEY_ENTER:
				newline();
				break;
			case KEY_BACKSPACE:
				backspace();
				break;
			case KEY_DEL:
				del();
				break;
			case KEY_TAB:
				insert_char('\t');
				break;
			case ctrl('x'):
				cut_selection();
				break;
			case ctrl('v'):
				paste_selection();
				break;
			case ctrl('o'):
				open_editor(ed);
				ed = env->current;
				break;
			case ctrl('n'):
				new_editor(ed);
				ed = env->current;
				break;
			case ctrl('w'):
				close_editor(ed);
				ed = env->current;
				break;
			case ctrl('s'):
				save_editor(ed);
				break;
			case ctrl('p'):
				pipe_command(ed);
				break;
#endif
			}
		}
	}
}

// globals
struct env env;

//
// main
//
int main() {
	// Initialize ncurses.
	//initscr();
	initkeys();

	int rc;
	int i;
	char file[80];
	sigset_t blocked_sigmask, orig_sigmask;

	//struct termios tio;
	//struct termios orig_tio;

	memset(&env, 0, sizeof(env));
	struct editor *ed = create_editor(&env);
	cout<<"Enter filename:";
	gets(file);
		rc = load_file(ed,file );
		if (rc < 0 && errno == ENOENT)
			rc = new_file(ed, file);
		
	
	if (env.current == NULL) {
		struct editor *ed = create_editor(&env);
		if (isatty(fileno(stdin))) {
			new_file(ed, "");
		} else {
			read_from_stdin(ed);
		}
	}

	
	get_console_size(&env);


	for (;;) {
		if (!env.current)
			break;
		edit(env.current);
		if (quit(&env))
			break;
	}

	gotoxy(0, env.lines + 1);
	outstr(RESET_COLOR CLREOL);

	//tcsetattr(0, TCSANOW, &orig_tio);

	while (env.current)
		delete_editor(env.current);

	if (env.clipboard)
		free(env.clipboard);
	if (env.search)
		free(env.search);
	if (env.linebuf)
		free(env.linebuf);

	setbuf(stdout, NULL);
	sigprocmask(SIG_SETMASK, &orig_sigmask, NULL);

	// Close ncurses.
	//endwin();

	return 0;
	
}
