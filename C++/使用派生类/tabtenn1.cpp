#include "tabtenn1.h"
#include <iostream>
using namespace std;

TableTennisPlayer::TableTennisPlayer( const string &fn, const string &ln,
                                     bool ht ) : firstname( fn ), lastname( ln ), hasTable( ht ){}

void TableTennisPlayer::Name() const{
    cout << lastname << ", " << firstname;
}

RatedPlayer::RatedPlayer( unsigned int r, const string &fn, const string &ln,
                          bool ht ) : TableTennisPlayer( fn, ln, ht ){
    rating = r;
}

RatedPlayer::RatedPlayer( unsigned int r, const TableTennisPlayer &tp ) :
                          TableTennisPlayer( tp ), rating( r ){}

