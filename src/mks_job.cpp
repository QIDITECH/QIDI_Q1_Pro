/*
#include <stdio.h>
#include <sqlite3.h>

sqlite3 *db = NULL;

int mks_open_database() {
    int rc = 0;

    rc = sqlite3_open("/root/mks.db", &db);

    if (rc != SQLITE_OK) {
        printf("Open db error :%s", sqlite3_errmsg(db));
        return -1;
    }

    return 0;
}

void mks_close_database() {
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }
}
*/