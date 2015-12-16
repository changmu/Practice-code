
#include <iostream>
#include <string>
#define CLIENTS 4
using namespace std;

class Acctabc{
private:
    string fullName;
    long acctNum;
    double balance;
protected:
    struct Formatting{
        ios_base::fmtflags flag;
        streamsize pr;
    };
    const string &FullName() const { return fullName; }
    long AcctNum() const { return acctNum; }
    Formatting SetFormat() const;
    void Restore( Formatting &f ) const;
public:
    Acctabc( const string &s = "Nullbody", long an = -1, double bal = 0.0 );
    void Deposit( double amt );
    virtual void Withdraw( double amt ) = 0;
    double Balance() const { return balance; }
    virtual void ViewAcct() const = 0;
    virtual ~Acctabc(){}
};

class Brass : public Acctabc{
public:
    Brass( const string &s = "Nullbody", long an = -1, double bal = 0.0 ) : Acctabc( s, an, bal ){}
    virtual void Withdraw( double amt );
    virtual void ViewAcct() const;
    virtual ~Brass(){}
};

class Brassplus : public Acctabc{
private:
    double maxloan;
    double rate;
    double owesbank;
public:
    Brassplus( const string &s = "Nullbody", long an = -1, double bal = 0.0, double ml = 500, double r = 0.10 );
    Brassplus( const Brass &ba, double ml = 500, double r = 0.1 );
    virtual void ViewAcct() const;
    virtual void Withdraw( double amt );
    void ResetMax( double m ) { maxloan = m; }
    void ResetRate( double r ) { rate = r; }
    void ResetOwes() { owesbank = 0; }
};


Acctabc::Acctabc( const string &s, long an, double bal ){
    fullName = s;
    acctNum = an;
    balance = bal;
}

void Acctabc::Deposit( double amt ){
    if( amt < 0 )
        cout << "Negative deposit not allowed; deposit is cancelled.\n";
    else
        balance += amt;
}

void Acctabc::Withdraw( double amt ){ balance -= amt; }

Acctabc::Formatting Acctabc::SetFormat() const{
    Formatting f;
    f.flag = cout.setf( ios_base::fixed, ios_base::floatfield );
    f.pr = cout.precision( 2 );
    return f;
}

void Acctabc::Restore( Formatting &f ) const{
    cout.setf( f.flag, ios_base::floatfield );
    cout.precision( f.pr );
}

void Brass::Withdraw( double amt ){
    if( amt < 0 )
        cout << "Withdrawl amount must be positive; withdrawl canceled.\n";
    else if( amt <= Balance() )
        Acctabc::Withdraw( amt );
    else
        cout << "Withdrawl amount of $" << amt
             << " exceeds your balance.\n"
             << "Withdrawl canceled.\n";
}

void Brass::ViewAcct() const{
    Formatting f = SetFormat();
    cout << "Brass Client: " << FullName() << endl;
    cout << "Account Number: " << AcctNum() << endl;
    cout << "Balance: $" << Balance() << endl;
    Restore( f );
}

Brassplus::Brassplus( const string &s, long an, double bal, double ml, double r ) : Acctabc( s, an, bal )
{
    maxloan = ml;
    owesbank = 0.0;
    rate = r;
}

Brassplus::Brassplus( const Brass &ba, double ml, double r ) : Acctabc( ba )
{
    maxloan = ml;
    owesbank = 0.0;
    rate = r;
}

void Brassplus::ViewAcct() const{
    Formatting f = SetFormat();
    cout << "Brassplus Client: " << FullName() << endl;
    cout << "Account Number: " << AcctNum() << endl;
    cout << "Balance: $" << Balance() << endl;
    cout << "Maximum loan: $" << maxloan << endl;
    cout << "Owed to bank: $" << owesbank << endl;
    cout.precision( 3 );
    cout << "Loan Rate: " << 100 * rate << "%\n";
    Restore( f );
}

void Brassplus::Withdraw( double amt ){
    Formatting f = SetFormat();

    double bal = Balance();
    if( amt <= bal )
        Acctabc::Withdraw( amt );
    else if( amt <= bal + maxloan - owesbank ){
        double advance = amt - bal;
        owesbank += advance * ( 1.0 + rate );
        cout << "Bank advance: $" << advance << endl;
        cout << "Finance charge: $" << advance * rate << endl;
        Deposit( advance );
        Acctabc::Withdraw( amt );
    }
    else
        cout << "Credit limit exceeded. Transaction cancelled.\n";
    Restore( f );
}

int main(){
    Acctabc *p_clients[CLIENTS];
    string temp;
    long tempnum;
    double tempbal;
    char kind;
    for( int i = 0; i != CLIENTS; ++i ){
        cout << "Enter client's name: ";
        getline( cin, temp );
        cout << "Enter client's account number: ";
        cin >> tempnum;
        cout << "Enter opening balance: $";
        cin >> tempbal;
        cout << "Enmter 1 for Brass Account or 2 for Brassplus Account: ";
        while( cin >> kind && ( kind != '1' && kind != '2' ) )
            cout << "Enter either 1 or 2: ";
        if( kind == '1' )
            p_clients[i] = new Brass( temp, tempnum, tempbal );
        else{
            double tmax, trate;
            cout << "Enter the overdraft limit: $";
            cin >> tmax;
            cout << "Enter the interest rate as a decimal fraction: ";
            cin >> trate;
            p_clients[i] = new Brassplus( temp, tempnum, tempbal, tmax, trate );
        }
        while( cin.get() != '\n' )
            ;
    }
    cout << endl;
    for( int i = 0; i != CLIENTS; ++i ){
        p_clients[i]->ViewAcct();
        cout << endl;
    }
    for( int i = 0; i != CLIENTS; ++i )
        delete p_clients[i];
    cout << "Done.\n";
    return 0;
}
