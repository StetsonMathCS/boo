#include <iostream>
#include "database/sqlite3.h"
#include "database/activity.h"
#include "database/database.h"
#include <cstdlib>
#include <string>
#include <cstring>

using namespace std;

Activity::Activity(size_t _id, string act_name, size_t event_id, string _status) {
    this->id = _id;
    this->name = act_name;
    this->eventId = event_id;
    this->status = _status;
}

 Activity* Activity::createActivity(string activity_name, size_t event_id, string activity_status) {
    int retval;
    sqlite3* db = Database::openDatabase();
    sqlite3_stmt *s;
    const char *sql = "INSERT INTO activities (name, eventid, status) VALUES (?, ?, ?)";
    retval = sqlite3_prepare(db, sql, strlen(sql), &s, NULL);
    if (retval != SQLITE_OK) {
        cout << "Error in preparing insert statement for activity " << sqlite3_errcode(db) << endl;
        return NULL;
    }
    retval = sqlite3_bind_text(s, 1, activity_name.c_str(), activity_name.size(), SQLITE_STATIC);
    if (retval != SQLITE_OK) {
        cout << "Error in binding SQL statement " << sql << endl;
        return NULL;
    }

   retval = sqlite3_bind_int(s, 2, event_id);

   retval = sqlite3_bind_text(s, 3, activity_status.c_str(), activity_status.size(), SQLITE_STATIC);
   if (retval != SQLITE_OK) {
       cout << "Error in binding SQL statement " << sql << endl;
       return NULL;
   }
    if (retval != SQLITE_OK) {
        cout << "Error in binding SQL statement " << sql << endl;
        return NULL;
    }
    if (sqlite3_step(s) != SQLITE_DONE) {
        cout << "Error executing SQL statement " << sql << ": " << sqlite3_errcode(db) << endl;
        return NULL;
    }

    //select statement to get the event id
    sql = "SELECT activityid FROM activities WHERE name = ? AND eventid = ? AND status = ?";
    retval = sqlite3_prepare(db, sql, strlen(sql), &s, NULL);
    if (retval != SQLITE_OK) {
        cout << "Error preparing select statement for activity " << sqlite3_errcode(db) << endl;
        return NULL;
    }
    retval = sqlite3_bind_text(s, 1, activity_name.c_str(), activity_name.size(), SQLITE_STATIC);
    if (retval != SQLITE_OK) {
        cout << "Error in binding SQL statement " << sql << endl;
        return NULL;
    }
    retval = sqlite3_bind_int(s, 2, event_id);
    if (retval != SQLITE_OK) {
        cout << "Error in binding SQL statement " << sql << endl;
        return NULL;
    }

    retval = sqlite3_bind_text(s, 3, activity_status.c_str(), activity_status.size(), SQLITE_STATIC);
    if (retval != SQLITE_OK) {
        cout << "Error in binding SQL statement " << sql << endl;
        return NULL;
    }

    size_t act_id = 0;
    if (sqlite3_step(s) == SQLITE_ROW) {
        act_id = (size_t)sqlite3_column_int(s, 0);
    }

	//needs to return pointer to the activity created
    Activity* a = new Activity(act_id, activity_name , event_id , activity_status);

	return a;
}
 Activity* Activity::loadActivityById(size_t _id) {
     sqlite3 *db = Database::openDatabase();
     int retval;
     sqlite3_stmt *s;
     string name, status;
     size_t eventId = -1;


     const char *sql = "SELECT * FROM activities WHERE activityid = ?";
     retval = sqlite3_prepare(db, sql, strlen(sql), &s, NULL);
     if (retval != SQLITE_OK) {
         cout << "Error preparing select statement for activities " << sqlite3_errcode(db) << endl;
         return NULL;
     }
     retval = sqlite3_bind_int(s, 1, _id);
     if (retval != SQLITE_OK) {
         cout << "Error in binding value to SQL statement " << sql << endl;
         return NULL;
     }
     if(sqlite3_step(s) == SQLITE_ROW) {
         name = string(reinterpret_cast<const char*>(sqlite3_column_text(s, 1)));
         eventId = sqlite3_column_int(s, 2);
         status = string(reinterpret_cast<const char*>(sqlite3_column_text(s, 3)));
     }


     Activity *a = new Activity(_id, name, eventId, status);
     return a;
 }


void Activity:: setEventId(size_t newid) {
    this->eventId = newid;
    sqlite3* db = Database::openDatabase();
    int retval;

    sqlite3_stmt* s;
    const char* sql = "UPDATE activities SET eventid = ? WHERE activityid = ?";
    retval = sqlite3_prepare(db, sql, strlen(sql), &s, NULL);
    if (retval != SQLITE_OK) {
        cout << "Error in preparing update statement for updating event id field in activity table " << sqlite3_errcode(db) << endl;
        return;
    }
    retval = sqlite3_bind_int(s, 2, this->id);
    if (retval != SQLITE_OK) {
        cout << "Error in binding int to SQL statement " << sql << endl;
        return;
    }
    retval = sqlite3_bind_int(s, 1, this->eventId);
    if (retval != SQLITE_OK) {
        cout << "Error in binding text to SQL statment " << sql << endl;
        return;
    }
    if (sqlite3_step(s) != SQLITE_DONE) {
        cout << "Error executing SQL statement << " << sql << endl;
        return;
    }
    sqlite3_reset(s);
}
void Activity:: setActivityName(string newname) {
    this->name = newname;
    sqlite3* db = Database::openDatabase();
    int retval;

    sqlite3_stmt* s;
    const char* sql = "UPDATE activities SET name = ? WHERE activityid = ?";
    retval = sqlite3_prepare(db, sql, strlen(sql), &s, NULL);
    if (retval != SQLITE_OK) {
        cout << "Error in preparing update statement for updating activity name field in activity table " << sqlite3_errcode(db) << endl;
        return;
    }
    retval = sqlite3_bind_text(s, 1, name.c_str(), name.size(), SQLITE_STATIC);
    if (retval != SQLITE_OK) {
        cout << "Error in binding text to SQL statment " << sql << endl;
        return;
    }
    retval = sqlite3_bind_int(s, 2, this->id);
    if (retval != SQLITE_OK) {
        cout << "Error in binding int to SQL statement " << sql << endl;
        return;
    }
    if (sqlite3_step(s) != SQLITE_DONE) {
        cout << "Error executing SQL statement << " << sql << endl;
        return;
    }
    sqlite3_reset(s);
}

void Activity::setStatus(string _status) {
    this->status = _status;

    sqlite3* db = Database::openDatabase();
    int retval;
    sqlite3_stmt* s;

    const char* sql = "UPDATE activities SET status = ? WHERE activityid = ?";
    retval = sqlite3_prepare(db, sql, strlen(sql), &s, NULL);
    if (retval != SQLITE_OK) {
        cout << "Error in preparing update statement for activity status field in activities: error code " << sqlite3_errcode(db) << endl;
        return;
    }
    retval = sqlite3_bind_text(s, 1, status.c_str(), status.size(), SQLITE_STATIC);
    if (retval != SQLITE_OK) {
        cout << "Error binding text to SQL statement " << sql << endl;
        return;
    }
    retval = sqlite3_bind_int(s, 2, id);
    if (retval != SQLITE_OK) {
        cout << "Error binding int to SQL statement " << sql << endl;
        return;
    }
    if (sqlite3_step(s) != SQLITE_DONE) {
        cout << "Error executing SQL statement " << sql << endl;
        return;
    }
    sqlite3_reset(s);
}
Activity* Activity::searchByName(string  _name) {
    sqlite3* db = Database::openDatabase();
    int retval;
    sqlite3_stmt *s;
    string __name, status;
    size_t eventId = -1;
    size_t act_id = -1;


    const char *sql = "SELECT * FROM activities WHERE name LIKE '%' || ? || '%'";
    retval = sqlite3_prepare(db, sql, strlen(sql), &s, NULL);
    if (retval != SQLITE_OK) {
        cout << "Error preparing select statement for activities " << sqlite3_errcode(db) << endl;
        return NULL;
    }

    retval = sqlite3_bind_text(s, 1, _name.c_str(), _name.size(), SQLITE_STATIC);
    if (retval != SQLITE_OK) {
        cout << "Error binding text to SQL statement " << sql << endl;
        return;
    }

    if(sqlite3_step(s) == SQLITE_ROW) {
        act_id = sqlite3_column_int(s, 0);
        __name =  sqlite3_column_int(s, 1);
        eventId = sqlite3_column_int(s, 2);
        status = string(reinterpret_cast<const char*>(sqlite3_column_text(s, 3)));
    }else {
        cout<<"couldn't find a match"<<endl;

    }

    Activity *a = new Activity(act_id, __name, eventId, status);
    return a;

}

void Activity::setPreReq(Activity* pre) {

    //TODO
}
void Activity::addCheckins(Checkin *checkin) {
     myAttendees.push_back(checkin);
}

string Activity::getStatus() {
    return this->status;
}
size_t Activity:: getId() {
    return this->id;
}

size_t Activity:: getEventId() {
    return this->eventId;
}

vector<Checkin*> Activity:: getCheckins() {
    return this->myAttendees;
}

string Activity:: getActivityName() {
    return this->name;
}

Activity::~Activity() {

}
