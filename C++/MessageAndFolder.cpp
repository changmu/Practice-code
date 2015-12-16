#include <iostream>
#include <string>
#include <memory>
#include <set>

using std::string;
using std::set;

class Message {
    friend class Folder;
public:
    explicit Message(const string &str = ""): contents(str) {}
    Message(const Message&);
    Message& operator=(const Message&);
    ~Message();
    void save(Folder &);
    void remove(Folder &);
private:
    string contents;
    set<Folder *> folders;
    void addToFolders(const Message &);
    void removeFromFolders();
};

void Message::save(folders &f)
{
    folders.insert(&f);
    f.addMessage(this);
}

void Message::remove(Folder &f)
{
    folders.erase(&f);
    f.removeMessage(this);
}

void Message::addToFolders(const Message &m)
{
    for (auto f: m.folders)
        f->addMessage(this);
}

Message::Message(const Message &m)
    : contents(m.contents), folders(m.folders)
{
    addToFolders(m);
}

void Message::removeFromFolders()
{
    for (auto f : folders)
        f->removeMessage(this);
}

Message::~Message()
{
    removeFromFolders();
}

Message& Message::operator=(const Message &rhs)
{
    removeFromFolders();
    contents = rhs.contents;
    folders = rhs.folders;
    addToFolders(rhs);
    return *this;
}

void swap(Message &lhs, Message &rhs)
{
    using std::swap;
    for (auto f: lhs.folders)
        r->removeMessage(&lhs);
    for (auto f: rhs.folders)
        r->removeMessage(&rhs);
    swap(lhs.folders, rhs.folders);
    swap(lhs.contents, rhs.contents);
    for (auto f: lhs.folders)
        f->addMessage(&lhs);
    for (auto f: rhs.folders)
        f->addMessage(&rhs);
}

class Folder {
    friend void swap(Message&, Message&);
    friend void swap(Folder&, Folder&);
    friend class Message;
public:
    Folder() = default;
    Folder(const Folder&);
    Folder& operator=(const Folder&);
    ~Folder();
private:
    set<Message *> msgs;

    void addToMessage(const Folder &);
    void removeToMessage();
    void addMsg(Message *m) { msgs.insert(m); }
    void remMsg(Message *m) { msgs.erase(m); }
};

void swap(Folder &lhs, Folder &rhs)
{
    using std::swap;
    for (auto m: lhs.msgs)
        m->removeFromFolders(&lhs);

    for (auto m: rhs.msgs)
        m->removeFromFolders(&rhs);

    swap(lhs.msgs, rhs.msgs);

    for (auto m: lhs.msgs)
        m->addToFolders(&lhs);

    for (auto m: rhs.msgs)
        m->addToFolders(&rhs);
}
