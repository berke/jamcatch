/* CAC3200 Elementary Abstract 32-bit CPU
 *   by Berke Durak <berke@gsu.linux.org.tr>
 *
 * con3200.c - X11 system console emulator
 */

/* WISH: using a separate subwindow for the LCD display could have been nicer */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xcms.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>

#include "types.h"
#include "display.h"
#include "menu.h"
#include "gogol.h"
#include "misc.h"
#include "font.h"

/* window */

#define WIN_WIDTH 590
#define WIN_HEIGHT 84

Display *d;/*{{{*/
int s;
Window w;
long evmask;
Visual *vi;
Colormap cmap;
Cursor curs;
XGCValues gcv;
Atom dw;    /* deletewindow protocol */
short width = WIN_WIDTH, height = WIN_HEIGHT;/*}}}*/

/* lcd screen */

XGCValues lcd_gcv;/*{{{*/
GC lcd_gc_b;
GC lcd_gc00, lcd_gc01;
GC lcd_gc10, lcd_gc11;/*}}}*/
Pixmap lcd_fontpix;/*{{{*/
Pixmap lcd_pix;/*}}}*/
#define LCD_PIXW 3/*{{{*/
#define LCD_PIXH 3
#define LCD_CHARW 6
#define LCD_CHARS 256
#define LCD_CHARSX 2
#define LCD_CHARSY 3
#define LCD_CHARH 8
#define LCD_COLS 16
#define LCD_ROWS 2
#define LCD_BORW 4
#define LCD_BORH 4
#define LCD_TOTW (LCD_COLS*(LCD_PIXW*LCD_CHARW+LCD_CHARSX)+2*LCD_BORW)
#define LCD_TOTH (LCD_ROWS*(LCD_PIXH*LCD_CHARH+LCD_CHARSY)+2*LCD_BORH)
#define LCD_ORIGX 50
#define LCD_ORIGY 10/*}}}*/

char lcd_mem[LCD_ROWS][LCD_COLS];

/* color map allocation */

typedef struct _color_description {/*{{{*/
  int n;
  unsigned char r,g,b;
} color_description;

enum {  COLOR_BACKGROUND,
  COLOR_FOREGROUND,
  COLOR_LCD_BACKGROUND,
  COLOR_LCD_BORDERHI,
  COLOR_LCD_BORDERLO,
  COLOR_LCD_FOREGROUND00,
  COLOR_LCD_FOREGROUND01,
  COLOR_LCD_FOREGROUND10,
  COLOR_LCD_FOREGROUND11,
  COLOR_PAN_TEXT,
  COLOR_PAN_CURSOR,
  COLOR_PAN_A_CURSOR,
  COLOR_PAN_CAN,
  COLOR_PAN_A_CAN,
  COLOR_PAN_ACK,
  COLOR_PAN_A_ACK,
  COLOR_PAN_NAK,
  COLOR_PAN_A_NAK,
  COLOR_COUNT };

static color_description cdescr[COLOR_COUNT] = {
  { COLOR_BACKGROUND       ,  80,  80,  80, },
  { COLOR_FOREGROUND       , 255, 255, 255, },
  { COLOR_LCD_BACKGROUND   , 230, 210, 180, },
  { COLOR_LCD_BORDERLO     ,  40,  40,  40, },
  { COLOR_LCD_BORDERHI     , 200, 200, 200, },
  { COLOR_LCD_FOREGROUND00 , 220, 200, 150, },
  { COLOR_LCD_FOREGROUND01 , 210, 190, 160, },
  { COLOR_LCD_FOREGROUND10 , 180, 150, 130, },
  { COLOR_LCD_FOREGROUND11 , 120,  80,  40, },
  { COLOR_PAN_TEXT         , 255, 255, 255, },
  { COLOR_PAN_CURSOR       ,  10,  40, 100, },
  { COLOR_PAN_A_CURSOR     ,  40,  70, 100, },
  { COLOR_PAN_CAN          , 100,  20,  10, },
  { COLOR_PAN_A_CAN        , 140,  40,  20, },
  { COLOR_PAN_ACK          ,  10, 120,  30, },
  { COLOR_PAN_A_ACK        ,  10, 160,  30, },
  { COLOR_PAN_NAK          , 140,  80,  10, },
  { COLOR_PAN_A_NAK        , 180, 120,  30, },
};

unsigned long ctable[COLOR_COUNT];/*}}}*/

/* UP,DOWN,LEFT,RIGHT,ACK,NAK,CAN
 * 7-button panel */

#define PAN_ORIGX 10
#define PAN_ORIGY 10
#define PAN_FONTNAME "-*-fixed-*-*-*-*-14-*-*-*-*-*-*-*"

typedef struct panel_button {/*{{{*/
  int n;
  int x,y;
  int w,h;

  int col, acol;
  void (*render_shape) (struct panel_button *);
  int shape;
  
  char *text;
} panel_button;/*}}}*/
XFontStruct *pan_font;/*{{{*/
XGCValues pan_gcv;
GC pan_gc;
int pan_menu_j;/*}}}*/

void render_rectangle (panel_button *pb);
void render_triangle (panel_button *pb);
void render_circle (panel_button *pb);

enum { PAN_CAN, PAN_ACK, PAN_NAK, PAN_LEFT, PAN_RIGHT, PAN_UP, PAN_DOWN, PAN_CNT };/*{{{*/

panel_button *panel[PAN_CNT+1]={/*{{{*/
  &(panel_button) {
    PAN_CAN,
    0, 0,
    30, 30,
    COLOR_PAN_CAN, COLOR_PAN_A_CAN,
    render_circle, 0,
    "CAN",
  },
  &(panel_button) {
    PAN_ACK,
    (LCD_ORIGX-PAN_ORIGX)+LCD_TOTW+10, 0,
    30, 20,
    COLOR_PAN_ACK, COLOR_PAN_A_ACK,
    render_rectangle, 0,
    "ACK",
  },
  &(panel_button) {
    PAN_NAK,
    (LCD_ORIGX-PAN_ORIGX)+LCD_TOTW+10+30+10, 0,
    30, 20,
    COLOR_PAN_NAK, COLOR_PAN_A_NAK,
    render_rectangle, 0,
    "NAK",
  },
  &(panel_button) {
    PAN_LEFT,
    (LCD_ORIGX-PAN_ORIGX)+LCD_TOTW+10, 30,
    30, 30,
    COLOR_PAN_CURSOR, COLOR_PAN_A_CURSOR,
    render_triangle, 0,
    0,
  },
  &(panel_button) {
    PAN_RIGHT,
    (LCD_ORIGX-PAN_ORIGX)+LCD_TOTW+10+30+10, 30,
    30, 30,
    COLOR_PAN_CURSOR, COLOR_PAN_A_CURSOR,
    render_triangle, 1,
    0,
  },
  &(panel_button) {
    PAN_UP,
    (LCD_ORIGX-PAN_ORIGX)+LCD_TOTW+10+30+10+30+10, 0,
    30, 30,
    COLOR_PAN_CURSOR, COLOR_PAN_A_CURSOR,
    render_triangle, 2,
    0,
  },
  &(panel_button) {
    PAN_DOWN,
    (LCD_ORIGX-PAN_ORIGX)+LCD_TOTW+10+30+10+30+10, 40,
    30, 30,
    COLOR_PAN_CURSOR, COLOR_PAN_A_CURSOR,
    render_triangle, 3,
    0,
  },
  0
};/*}}}*/

unsigned long pan_active = 0x00000000;

#define PAN_MASK(pb) (1<<pb->n)
#define PAN_ISACTIVE(pb) (pan_active & (1<<pb->n))
#define PAN_ACTIVE_SET(pb) (pan_active |= (1<<pb->n))
#define PAN_ACTIVE_CLEAR(pb) (pan_active &= ~(1<<pb->n))/*}}}*/

/* generic renderinc functions */

void render_rectangle (panel_button *pb)/*{{{*/
{
  XFillRectangle (d, w, pan_gc, PAN_ORIGX+pb->x, PAN_ORIGY+pb->y, pb->w, pb->h);
}/*}}}*/
void render_circle (panel_button *pb)/*{{{*/
{
  XFillArc (d, w, pan_gc, PAN_ORIGX+pb->x, PAN_ORIGY+pb->y, pb->w, pb->h, 0, 360*64);
}/*}}}*/
void render_triangle (panel_button *pb)/*{{{*/
{
  int i;
  XPoint tri[3];
  
  switch (pb->shape) {
    case 0:  /* LEFT */
      tri[0].x =     0; tri[0].y = pb->h/2;
      tri[1].x = pb->w; tri[1].y = 0;
      tri[2].x = pb->w; tri[2].y = pb->h;
      break;
    case 1: /* RIGHT */
      tri[0].x = pb->w; tri[0].y = pb->h/2;
      tri[1].x =     0; tri[1].y = 0;
      tri[2].x =     0; tri[2].y = pb->h;
      break;
    case 2: /* UP */
      tri[0].x = pb->w/2; tri[0].y = 0;
      tri[1].x = pb->w; tri[1].y = pb->h;
      tri[2].x = 0; tri[2].y = pb->h;
      break;
    case 3: /* DOWN */
      tri[0].x =     0; tri[0].y = 0;
      tri[1].x = pb->w; tri[1].y = 0;
      tri[2].x = pb->w/2; tri[2].y = pb->h;
      break;
  }
  
  for (i = 0; i<3; tri[i].x += PAN_ORIGX+pb->x, tri[i].y += PAN_ORIGY+pb->y, i++);
  XFillPolygon (d, w, pan_gc, tri, 3, Complex, CoordModeOrigin);
}/*}}}*/

/* redraw panel... */

void redraw_button (panel_button *pb)/*{{{*/
{
  pan_gcv.foreground = PAN_ISACTIVE (pb)?ctable[pb->acol]:ctable[pb->col];
  XChangeGC (d, pan_gc, GCForeground, &pan_gcv);
  pb->render_shape (pb);

  if (pb->text) {
    int x, y;
    int dr, far, fdr;
    XCharStruct or;
    
    XTextExtents (pan_font, pb->text, strlen (pb->text),
          &dr, &far, &fdr, &or);
    
    x = (PAN_ORIGX + pb->x) +
      (pb->w - or.width)/2;
    y = (PAN_ORIGY + pb->y) +
      (pb->h - (or.ascent + or.descent))/2 + or.ascent;
    
    pan_gcv.background = PAN_ISACTIVE (pb)?ctable[pb->acol]:ctable[pb->col];
    pan_gcv.foreground = ctable[COLOR_PAN_TEXT];
    XChangeGC (d, pan_gc, GCForeground|GCBackground, &pan_gcv);
    XDrawImageString (d, w, pan_gc,
          x,
          y,
          pb->text,
          strlen (pb->text));
  }
}/*}}}*/
void redraw_panel (void)/*{{{*/
{
  int i;
  
  for (i = 0; panel[i]; i++) {
    redraw_button (panel[i]);
  }
}/*}}}*/
panel_button *find_button (int x, int y)/*{{{*/
{
  int i;
  panel_button *pb;
  
  x -= PAN_ORIGX;
  y -= PAN_ORIGY;
  
  for (i = 0; panel[i]; i++) {
    pb = panel[i];
    /* debugf ("x=%d, y=%d, pb->x=%d, pb->y=%d, pb->w=%d, pb->h=%d",
      x,y,pb->x,pb->y,pb->w,pb->h); */
    if (x >= pb->x && x < pb->x+pb->w &&
        y >= pb->y && y < pb->y+pb->h) return pb;
  }
  return 0;
}/*}}}*/

/* init panel stuff... */

void init_panel ()/*{{{*/
{
  pan_gcv.plane_mask = AllPlanes;
  pan_gcv.line_width = 1;
  pan_gcv.background = ctable[COLOR_BACKGROUND];
  pan_gcv.line_style = LineSolid;
  pan_gcv.function = GXcopy;

  pan_gc = XCreateGC (  d, w, \
        GCPlaneMask | \
        GCFunction | \
        GCLineStyle | \
        GCBackground | \
        GCLineWidth, \
        &pan_gcv);
  
  if (!pan_gc) {
    errorf (0, "init_panel: XCreateGC failed");
    exit (EXIT_FAILURE);
  }
  
  pan_font = XLoadQueryFont (d, PAN_FONTNAME);
  if (!pan_font) {
    errorf (0, "init_panel: XLoadQueryFont \"%s\" failed", PAN_FONTNAME);
    exit (EXIT_FAILURE);
  }
  
  XSetFont (d, pan_gc, pan_font->fid);
}/*}}}*/

/* lcd */

void draw_lcd_pixel (int x, int y, int o)/*{{{*/
{
  if (o) {
    XFillRectangle (d, lcd_fontpix, lcd_gc10, x, y, LCD_PIXW, LCD_PIXH);
    XFillRectangle (d, lcd_fontpix, lcd_gc11, x, y, LCD_PIXW-1, LCD_PIXH-1);
  } else {
    XFillRectangle (d, lcd_fontpix, lcd_gc00, x, y, LCD_PIXW, LCD_PIXH);
    XFillRectangle (d, lcd_fontpix, lcd_gc01, x, y, LCD_PIXW-1, LCD_PIXH-1);
  }
}/*}}}*/
void make_lcd_font ()/*{{{*/
{
  int a;
  int x, y;
  
  for (a = 0; a<LCD_CHARS; a++) {
    for (y = 0; y<LCD_CHARH; y++) {
      for (x = 0; x<LCD_CHARW; x++) {
        draw_lcd_pixel (LCD_PIXW*(a*LCD_CHARW+x),
            LCD_PIXH*y,
            font[a][y][x]);
      }
    }
  }
}/*}}}*/
void copy_lcd_char (int a, int c, int r)/*{{{*/
{
  XCopyArea (  d,
      lcd_fontpix,
      lcd_pix,
      lcd_gc_b,
      LCD_PIXW*(a*LCD_CHARW), 0,
      LCD_PIXW*LCD_CHARW, LCD_PIXH*LCD_CHARH,
      LCD_BORW+c*(LCD_PIXW*LCD_CHARW+LCD_CHARSX),
      LCD_BORH+r*(LCD_PIXH*LCD_CHARH+LCD_CHARSY)
      );
}/*}}}*/
void draw_lcd_frame ()/*{{{*/
{
  XFillRectangle (  d,
        lcd_pix,
        lcd_gc_b,
        0, 0,
        LCD_TOTW, LCD_TOTH);

  lcd_gcv.foreground = ctable[COLOR_LCD_BORDERLO];
  XChangeGC (d, lcd_gc_b, GCForeground, &lcd_gcv);
  XDrawLine (d, lcd_pix, lcd_gc_b, 0, 0, LCD_TOTW-1, 0);
  XDrawLine (d, lcd_pix, lcd_gc_b, 0, 0, 0, LCD_TOTH-1);

  lcd_gcv.foreground = ctable[COLOR_LCD_BORDERHI];
  XChangeGC (d, lcd_gc_b, GCForeground, &lcd_gcv);
  XDrawLine (d, lcd_pix, lcd_gc_b, 0, LCD_TOTW-1, LCD_TOTW-1, LCD_TOTH-1);
  XDrawLine (d, lcd_pix, lcd_gc_b, LCD_TOTW-1, LCD_TOTH-1, 0, LCD_TOTH-1);

  lcd_gcv.foreground = ctable[COLOR_LCD_BACKGROUND];
  XChangeGC (d, lcd_gc_b, GCForeground, &lcd_gcv);
}/*}}}*/
void redraw_lcd ()/*{{{*/
{
  int c, r;

  for (c = 0; c<LCD_COLS; c++) {
    for (r = 0; r<LCD_ROWS; r++) {
      copy_lcd_char (lcd_mem[r][c], c, r);
    }
  }
}/*}}}*/
void copy_lcd ()/*{{{*/
{
  XCopyArea (  d,
      lcd_pix,
      w,
      lcd_gc_b,
      0,0,
      LCD_TOTW, LCD_TOTH,
      LCD_ORIGX, LCD_ORIGY
      );
  XFlush (d);
  
}/*}}}*/
/*{{{*/
#define LCD_CREATEGC XCreateGC (  d, w, \
          GCPlaneMask | \
          GCFunction | \
          GCLineStyle | \
          GCForeground | \
          GCBackground | \
          GCLineWidth, \
          &lcd_gcv)
/*}}}*/
void init_lcd ()/*{{{*/
{
  lcd_gcv.background = ctable[COLOR_LCD_BACKGROUND];
  lcd_gcv.plane_mask = AllPlanes;
  lcd_gcv.line_width = 1;
  lcd_gcv.line_style = LineSolid;

  lcd_gcv.function = GXcopy;

  lcd_gcv.foreground = ctable[COLOR_LCD_BACKGROUND  ]; lcd_gc_b = LCD_CREATEGC;
  lcd_gcv.foreground = ctable[COLOR_LCD_FOREGROUND00]; lcd_gc00 = LCD_CREATEGC;
  lcd_gcv.foreground = ctable[COLOR_LCD_FOREGROUND01]; lcd_gc01 = LCD_CREATEGC;
  lcd_gcv.foreground = ctable[COLOR_LCD_FOREGROUND10]; lcd_gc10 = LCD_CREATEGC;
  lcd_gcv.foreground = ctable[COLOR_LCD_FOREGROUND11]; lcd_gc11 = LCD_CREATEGC;
  
  if (! (  lcd_gc_b &&
    lcd_gc00 &&
    lcd_gc01 &&
    lcd_gc10 &&
    lcd_gc11)) {
    errorf (0, "init_lcd: some XCreateGC failed");
    exit (EXIT_FAILURE);
  }
  
  lcd_fontpix = XCreatePixmap (d, w, LCD_CHARW*LCD_PIXW*LCD_CHARS,
             LCD_CHARH*LCD_PIXH,
             DefaultDepth (d, s));
  lcd_pix     = XCreatePixmap (d, w, LCD_TOTW,
             LCD_TOTH,
             DefaultDepth (d, s));

  if (! (lcd_fontpix && lcd_pix)) {
    errorf (0, "init_lcd: some XCreatePixmap failed");
    exit (EXIT_FAILURE);
  }
  
  make_lcd_font ();
}/*}}}*/
void fill_ctable ()/*{{{*/
{
  int i;
  XColor col;
  
  for (i = 0; i<COLOR_COUNT; i++) {
    col.red = cdescr[i].r << 8;
    col.green = cdescr[i].g << 8;
    col.blue = cdescr[i].b << 8;
    if (!XAllocColor (d, cmap, &col)) {
      errorf (0, "fill_ctable: XAllocColor failed for %03d,%03d,%03d",
           cdescr[i].r, cdescr[i].r, cdescr[i].r);
    }
    ctable[cdescr[i].n] = col.pixel;
    /* debugf ("%03d,%03d,%03d -> 0x%08lx", cdescr[i].r, cdescr[i].r, cdescr[i].r, col.pixel); */
  }
}/*}}}*/

/* menu */

display_t pan_display;
void update_menus()/*{{{*/
{
  display_update(&pan_display, lcd_mem[0], lcd_mem[1], LCD_COLS);
}/*}}}*/

int handle ()/*{{{*/
{
  int refresh_needed = 0;
  int flush_needed = 0;
  int r = 0;
  int k = -1;
  short mx, my;
  panel_button *pb = 0;
  unsigned long pan_lastactive = pan_active;
  
  XEvent xe;
  KeySym ks;
  
  XNextEvent (d, &xe);
  
  switch (xe.type) {
    case ClientMessage:
      if (xe.xclient.format == 32 &&
        xe.xclient.data.l[0] == dw) {
        r = -1;  
      }
      break;
    case Expose:
      copy_lcd ();
      redraw_panel ();
      XFlush (d);
      break;
    case ButtonPress:
    case ButtonRelease:
      mx = xe.xbutton.x; my = xe.xbutton.y;
      if (xe.xbutton.button != Button1) break;
      pb = find_button (mx, my);
      if (pb) {
        if (PAN_ISACTIVE (pb) && xe.type == ButtonRelease) {
          PAN_ACTIVE_CLEAR (pb);
        } else if (!PAN_ISACTIVE (pb) && xe.type == ButtonPress) {
          PAN_ACTIVE_SET (pb);
        }
      }
      break;
    case KeyPress:
    case KeyRelease:
      ks = XLookupKeysym (&xe.xkey, xe.xkey.state);
      switch (ks) {
        case '+':
        case XK_Return:
        case ' ':
        case 'y':
          k = PAN_ACK;
          break;
        case XK_BackSpace:
        case XK_Delete:
        case 'q':
          exit(0);
          break;
        case 'n':
          k = PAN_NAK;
          break;
        case XK_Escape:
          k = PAN_CAN;
          break;
        case XK_Up:
          k = PAN_UP;
          break;
        case XK_Down:
          k = PAN_DOWN;
          break;
        case XK_Left:
          k = PAN_LEFT;
          break;
        case XK_Right:
          k = PAN_RIGHT;
          break;
        default:
          /*                    0123456 */
#if 0
          sprintf (lcd_mem[0], "Keysym %04lx '%c'     ",
              ks, (int) ks&0xff);
#endif
          break;
        /* default:
          printf ("keysym %ld\n", ks); */
      }
      
  }
  
  if (k != -1) {
    if (xe.type == KeyRelease) PAN_ACTIVE_CLEAR (panel[k]);
    else {
      PAN_ACTIVE_SET (panel[k]);
      switch(k) {
        case PAN_LEFT:
          display_apply(&pan_display, DK_LEFT);
          refresh_needed = 1;
          break;
        case PAN_RIGHT:
          display_apply(&pan_display, DK_RIGHT);
          refresh_needed = 1;
          break;
        case PAN_DOWN:
          display_apply(&pan_display, DK_DOWN);
          refresh_needed = 1;
          break;
        case PAN_UP:
          display_apply(&pan_display, DK_UP);
          refresh_needed = 1;
          break;
      }
    }
  }

  if(refresh_needed) {
    update_menus();
    redraw_lcd ();
    copy_lcd ();
    flush_needed = 1;
  }
    
  if (pan_active != pan_lastactive) {
    int i;
    panel_button *pb;
  
    for (i = 0; panel[i]; i++) {
      pb = panel[i];
      if (PAN_MASK (pb) & (pan_active ^ pan_lastactive)) {
        redraw_button (pb);
      }
    }
    flush_needed = 1;
  }

  if(flush_needed) XFlush (d);
  return r;
}/*}}}*/
/* main */
int main (int argc, char **argv)/*{{{*/
{
  XSizeHints xsh;
  XWMHints xwmh;
  XClassHint xch;

        int run = 1;
  
  d = XOpenDisplay (0);
  if (!d) {
    errorf (0, "could not open display");
    exit (EXIT_FAILURE);
  }
  
  s = XDefaultScreen (d);
  cmap = XDefaultColormap (d, s);
  fill_ctable ();

  w = XCreateSimpleWindow  (  d,
          RootWindow (d, s),
          0, 0,
          width, height,
          0,
          ctable[COLOR_BACKGROUND],
          ctable[COLOR_BACKGROUND]
        );
  if (!w) {
    errorf (0, "could not open window");
    exit (EXIT_FAILURE);
  }
  
  /* setup hints */
  
  xsh.flags = PMinSize | PMaxSize;
  xsh.min_width  = xsh.max_width  = WIN_WIDTH;
  xsh.min_height = xsh.max_height = WIN_HEIGHT;
  xwmh.input = 1;
  xwmh.flags = InputHint;
  xch.res_name = "con3200";    /* unique */
  xch.res_class = "con3200";
  XSetWMProperties (d, w, 0, 0, argv, argc, &xsh, &xwmh, &xch);
  XStoreName (d, w, "CON3200 / X11 CAC3200 system console emulator");

  /* set cursor */

  curs = XCreateFontCursor (d, XC_crosshair);
  XDefineCursor (d, w, curs);

  evmask =   ExposureMask | \
      KeyPressMask | \
      KeyReleaseMask | \
      ButtonPressMask | \
      ButtonReleaseMask | StructureNotifyMask;

  init_lcd ();
  init_panel ();
  
  XSelectInput (d, w, evmask);

  /* deletewindow protocol (ripped straight out from rxvt) */
  dw = XInternAtom (d, "WM_DELETE_WINDOW", 0);
  XSetWMProtocols (d, w, &dw, 1);
  
  XMapRaised (d, w);
  XFlush (d);
  
  /*                   01234567890123456789 */
  strcpy (lcd_mem[0], "con3200 v0.1        ");
  strcpy (lcd_mem[1], "                IDLE");

  /* Setup menu */
  display_init(&pan_display, &root, "v^<>  *");

  draw_lcd_frame ();
  update_menus();
  redraw_panel ();
  redraw_lcd ();
  copy_lcd ();

  while (run) {
    if (handle ()) run = 0;
  }

  
  XDestroyWindow (d, w);  
  XCloseDisplay (d);

  return 0;
}/*}}}*/
