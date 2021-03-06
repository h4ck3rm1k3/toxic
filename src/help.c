/*  help.c
 *
 *
 *  Copyright (C) 2014 Toxic All Rights Reserved.
 *
 *  This file is part of Toxic.
 *
 *  Toxic is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Toxic is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Toxic.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <string.h>

#include "windows.h"
#include "toxic.h"
#include "help.h"
#include "misc_tools.h"

#define HELP_MENU_HEIGHT 7
#define HELP_MENU_WIDTH 26

void help_init_menu(ToxWindow *self)
{
    if (self->help->win)
        delwin(self->help->win);

    int y2, x2;
    getmaxyx(self->window, y2, x2);

    if (y2 < HELP_MENU_HEIGHT || x2 < HELP_MENU_WIDTH)
        return;

    self->help->win = newwin(HELP_MENU_HEIGHT, HELP_MENU_WIDTH, 3, 3);
    self->help->active = true;
    self->help->type = HELP_MENU;
}

static void help_exit(ToxWindow *self)
{
    delwin(self->help->win);
    memset(self->help, 0, sizeof(Help));
}

static void help_init_window(ToxWindow *self, int height, int width)
{
    if (self->help->win)
        delwin(self->help->win);

    int y2, x2;
    getmaxyx(stdscr, y2, x2);

    height = MIN(height, y2);
    width = MIN(width, x2);

    self->help->win = newwin(height, width, 0, 0);
}

static void help_draw_menu(ToxWindow *self)
{
    WINDOW *win = self->help->win;

    wmove(win, 1, 1);

    wattron(win, A_BOLD | COLOR_PAIR(RED));
    wprintw(win, "       Help Menu\n");
    wattroff(win, A_BOLD | COLOR_PAIR(RED));

    wattron(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, " G");
    wattroff(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, "lobal commands\n");

    wattron(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, " C");
    wattroff(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, "hat commands\n");

    wattron(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, " K");
    wattroff(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, "ey bindings\n");

    wprintw(win, " E");
    wattron(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, "x");
    wattroff(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, "it menu\n");

    box(win, ACS_VLINE, ACS_HLINE);
    wrefresh(win);
}

static void help_draw_bottom_menu(WINDOW *win)
{
    int y2, x2;
    getmaxyx(win, y2, x2);
    wmove(win, y2 - 2, 1);

    wattron(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, " M");
    wattroff(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, "ain menu |");

    wprintw(win, " E");
    wattron(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, "x");
    wattroff(win, A_BOLD | COLOR_PAIR(BLUE));
    wprintw(win, "it");
}

static void help_draw_global(ToxWindow *self)
{
    WINDOW *win = self->help->win;

    wmove(win, 1, 1);

    wattron(win, A_BOLD | COLOR_PAIR(RED));
    wprintw(win, "Global Commands:\n");
    wattroff(win, A_BOLD | COLOR_PAIR(RED));

    wprintw(win, "  /add <id> <msg>            : Add friend with optional message\n");
    wprintw(win, "  /accept <n>                : Accept friend request\n");
    wprintw(win, "  /connect <ip> <port> <key> : Manually connect to a DHT node\n");
    wprintw(win, "  /status <type> <msg>       : Set status with optional note\n");
    wprintw(win, "  /note <msg>                : Set a personal note\n");
    wprintw(win, "  /nick <nick>               : Set your nickname\n");
    wprintw(win, "  /log <on> or <off>         : Enable/disable logging\n");
    wprintw(win, "  /groupchat                 : Create a group chat\n");
    wprintw(win, "  /myid                      : Print your ID\n");
    wprintw(win, "  /clear                     : Clear window history\n");
    wprintw(win, "  /close                     : Close the current chat window\n");
    wprintw(win, "  /quit or /exit             : Exit Toxic\n");

#ifdef _SUPPORT_AUDIO
    wattron(win, A_BOLD);
    wprintw(win, "\n Audio:\n");
    wattroff(win, A_BOLD);

    wprintw(win, "  /lsdev <type>              : List devices where type: in|out\n");
    wprintw(win, "  /sdev <type> <id>          : Set active device\n");
#endif /* _SUPPORT_AUDIO */

    help_draw_bottom_menu(win);

    box(win, ACS_VLINE, ACS_HLINE);
    wrefresh(win);
}

static void help_draw_chat(ToxWindow *self)
{
    WINDOW *win = self->help->win;

    wmove(win, 1, 1);

    wattron(win, A_BOLD | COLOR_PAIR(RED));
    wprintw(win, "Chat Commands:\n");
    wattroff(win, A_BOLD | COLOR_PAIR(RED));

    wprintw(win, "  /invite <n>                : Invite friend to a group chat\n");
    wprintw(win, "  /join                      : Join a pending group chat\n");
    wprintw(win, "  /sendfile <path>           : Send a file\n");
    wprintw(win, "  /savefile <n>              : Receive a file\n");

#ifdef _SUPPORT_AUDIO
    wattron(win, A_BOLD);
    wprintw(win, "\n Audio:\n");
    wattroff(win, A_BOLD);

    wprintw(win, "  /call                      : Audio call\n");
    wprintw(win, "  /cancel                    : Cancel call\n");
    wprintw(win, "  /answer                    : Answer incomming call\n");
    wprintw(win, "  /reject                    : Reject incoming call\n");
    wprintw(win, "  /hangup                    : Hangup active call\n");
    wprintw(win, "  /sdev <type> <id>          : Change active device\n");
    wprintw(win, "  /mute <type>               : Mute active device if in call\n");
    wprintw(win, "  /sense <n>                 : VAD sensitivity treshold\n");
#endif /* _SUPPORT_AUDIO */

    help_draw_bottom_menu(win);

    box(win, ACS_VLINE, ACS_HLINE);
    wrefresh(win);
}

static void help_draw_keys(ToxWindow *self)
{
    WINDOW *win = self->help->win;

    wmove(win, 1, 1);

    wattron(win, A_BOLD | COLOR_PAIR(RED));
    wprintw(win, "Key bindings:\n");
    wattroff(win, A_BOLD | COLOR_PAIR(RED));

    wprintw(win, "  Ctrl+O / Ctrl+P         : Navigate through the windows/tabs\n"); 
    wprintw(win, "  Page Up / Page Down     : Scroll window history one line\n");
    wprintw(win, "  Ctrl+F / Ctrl+V         : Scroll window history half a page\n");
    wprintw(win, "  Ctrl+H                  : Move to the bottom of window history\n");
    wprintw(win, "  Ctrl+[ / Ctrl+]         : Scroll peer list in groupchats\n");

    help_draw_bottom_menu(win);

    box(win, ACS_VLINE, ACS_HLINE);
    wrefresh(win);
}

void help_onKey(ToxWindow *self, wint_t key)
{
    switch(key) {
        case 'x':
        case T_KEY_ESC:
            help_exit(self);
            break;

        case 'c':
#ifdef _SUPPORT_AUDIO
            help_init_window(self, 19, 80);
#else
            help_init_window(self, 9, 80);
#endif
            self->help->type = HELP_CHAT;
            break;

        case 'g':
#ifdef _SUPPORT_AUDIO
            help_init_window(self, 21, 80);
#else
            help_init_window(self, 17, 80);
#endif
            self->help->type = HELP_GLOBAL;
            break;

        case 'k':
            help_init_window(self, 10, 80);
            self->help->type = HELP_KEYS;
            break;

        case 'm':
            help_init_menu(self);
            self->help->type = HELP_MENU;
            break;
    }
}

void help_onDraw(ToxWindow *self)
{
    curs_set(0);
    wclear(self->help->win);

    switch(self->help->type) {
        case HELP_MENU:
            help_draw_menu(self);
            return;

        case HELP_CHAT:
            help_draw_chat(self);
            break;

        case HELP_GLOBAL:
            help_draw_global(self);
            break;

        case HELP_KEYS:
            help_draw_keys(self);
            break;

        case HELP_GROUP:
            break;
    }
}
