/*  file_senders.c
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "toxic_windows.h"
#include "line_info.h"

FileSender file_senders[MAX_FILES];
uint8_t max_file_senders_index;

static void close_file_sender(int i)
{
    fclose(file_senders[i].file);
    memset(&file_senders[i], 0, sizeof(FileSender));

    int j;

    for (j = max_file_senders_index; j > 0; --j) {
        if (file_senders[j-1].active)
            break;
    }

    max_file_senders_index = j;
}

/* Should only be called on exit */
void close_all_file_senders(void)
{
    int i;

    for (i = 0; i < max_file_senders_index; ++i) {
        if (file_senders[i].active)
            fclose(file_senders[i].file);
    }
}

void do_file_senders(Tox *m)
{
    uint8_t msg[MAX_STR_SIZE];
    int i;

    for (i = 0; i < max_file_senders_index; ++i) {
        if (!file_senders[i].active)
            continue;

        ToxWindow *self = file_senders[i].toxwin;
        uint8_t *pathname = file_senders[i].pathname;
        int filenum = file_senders[i].filenum;
        int32_t friendnum = file_senders[i].friendnum;
        FILE *fp = file_senders[i].file;
        uint64_t current_time = get_unix_time();

        /* If file transfer has timed out kill transfer and send kill control */
        if (timed_out(file_senders[i].timestamp, current_time, TIMEOUT_FILESENDER)) {
            if (self->chatwin != NULL) {
                snprintf(msg, sizeof(msg), "File transfer for '%s' timed out.", pathname);
                line_info_add(self, NULL, NULL, NULL, msg, SYS_MSG, 0, 0);
                alert_window(file_senders[i].toxwin, WINDOW_ALERT_2, true);
            }

            tox_file_send_control(m, friendnum, 0, filenum, TOX_FILECONTROL_KILL, 0, 0);
            close_file_sender(i);
            continue;
        }

        while (true) {
            if (tox_file_send_data(m, friendnum, filenum, file_senders[i].nextpiece, 
                                   file_senders[i].piecelen) == -1)
                break;

            file_senders[i].timestamp = current_time;
            file_senders[i].piecelen = fread(file_senders[i].nextpiece, 1, 
                                             tox_file_data_size(m, friendnum), fp);

            /* refresh line with percentage complete */
            if (self->chatwin != NULL) {
                uint64_t size = file_senders[i].size;
                long double remain = (long double) tox_file_data_remaining(m, friendnum, filenum, 0);
                long double pct_remain = 100;

                if (remain)
                    pct_remain = (1 - (remain / size)) * 100;

                const uint8_t *name = file_senders[filenum].pathname;
                snprintf(msg, sizeof(msg), "File transfer for '%s' accepted (%.1Lf%%)", name, pct_remain);
                line_info_set(self, file_senders[filenum].line_id, msg);
            }

            if (file_senders[i].piecelen == 0) {
                if (self->chatwin != NULL) {
                    snprintf(msg, sizeof(msg), "File '%s' successfuly sent.", pathname);
                    line_info_add(self, NULL, NULL, NULL, msg, SYS_MSG, 0, 0);
                    alert_window(file_senders[i].toxwin, WINDOW_ALERT_2, true);
                }

                tox_file_send_control(m, friendnum, 0, filenum, TOX_FILECONTROL_FINISHED, 0, 0);
                close_file_sender(i);
                break;
            }
        }
    }
}
