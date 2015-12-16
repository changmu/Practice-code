#ifndef WORKERMI_H
#define WORKERMI_H
#include <string>
using namespace std;

class Worker{  //an abstract base class
    string fullname;
    long id;
protected:
    virtual void data() const;
    virtual void get();
public:
    Worker() : fullname( "no one" ), id( 0L ) {}
    Worker( const string &s, long n ) : fullname( s ), id( n ) {}
    virtual ~Worker() = 0; //pure virutal function
    virtual void Set() = 0;
    virtual void show() const = 0;
};

class Waiter : virtual public Worker{
    int panache;
protected:
    void data() const;
    void get();
public:
    Waiter() : Worker(), panache( 0 ) {}
    Waiter( const string &s, long n, int p = 0 )
            : Worker( s, n ), panache( p ) {}
    Waiter( const Worker &wk, int p = 0 )
            : Worker( wk ), panache( p ) {}
    void Set();
    void show() const;
};

class Singer : virtual public Worker{
protected:
    enum{ Vtypes = 7 };
    enum{ other, alto, contralto, soprano, bass, baritone, tenor };
    void data() const;
    void get();
private:
    static char *pv[Vtypes];
    int voice;
public:
    Singer() : Worker(), voice( other ) {}
    Singer( const string &s, long n, int v = other )
            : Worker( s, n ), voice( v ) {}
    Singer( const Worker &wk, int v = other )
            : Worker( wk ), voice( v ) {}
    void Set();
    void show() const;
};

class SingingWaiter : public Singer, public Waiter{
protected:
    void data() const;
    void get();
public:
    SingingWaiter() {}
    SingingWaiter( const string &s, long n, int p = 0, int v = other )
                   : Worker( s, n ), Waiter( s, n, p ), Singer( s, n, v ) {}
    SingingWaiter( const Worker &wk, int p = 0, int v = other )
                   : Worker( wk ), Waiter( wk, p ), Singer( wk, v ) {}
    SingingWaiter( const Worker &wk, int v = other )
                   : Worker( wk ), Waiter( wk ), Singer( wk, v ) {}
    SingingWaiter( const Singer &wk, int p = 0 )
                   : Worker( wk ), Waiter( wk, p ), Singer( wk ) {}
    void Set();
    void show() const;
};

#endif
