#include <memory>
#include <cstring>
#include <algorithm>
#include <iostream>

class String {
public:
    String(): String("") {}
    String(const char *);
    String(const String&);
    String& operator=(const String&);
    String(String &&) noexcept;
    String& operator=(String &&) noexcept;
    ~String();

    const char *c_str() const { return elements; }
    size_t size() const { return end - elements; }
    size_t length() const { return end - elements - 1; }

private:
    std::pair<char *, char *> alloc_n_copy(const char *, const char *);
    void range_initializer(const char *, const char *);
    void free();

    char *elements;
    char *end;
    std::allocator<char> alloc;
};

std::pair<char *, char *>
String::alloc_n_copy(const char *b, const char *e)
{
    auto str = alloc.allocate(e - b);
    return { str, std::uninitialized_copy(b, e, str) };
}

void String::range_initializer(const char *first, const char *last)
{
    auto newstr = alloc_n_copy(first, last);
    elements = newstr.first;
    end = newstr.second;
}

String::String(const char *s)
{
    range_initializer(s, s + strlen(s) + 1);
}

String::String(const String &rhs)
{
    range_initializer(rhs.elements, rhs.end);
    std::cout << "copy constructor" << std::endl;
}

void String::free()
{
    if (elements) {
        std::for_each(elements, end, [this](char &c) { alloc.destroy(&c); });
        alloc.deallocate(elements, end - elements);
    }
}

String::~String()
{
    free();
}

String& String::operator=(const String &rhs)
{
    auto newstr = alloc_n_copy(rhs.elements, rhs.end);
    free();
    elements = newstr.first;
    end = newstr.second;
    std::cout << "copy-assignment" << std::endl;
    return *this;
}

String::String(String &&s) noexcept : elements(s.elements), end(s.end)
{
    s.elements = s.end = nullptr;
}

String& String::operator=(String &&rhs) noexcept
{
    if (this != &rhs) {
        free();
        elements = rhs.elements;
        end = rhs.end;
        rhs.elements = rhs.end = nullptr;
    }
    return *this;
}

int main()
{
    String str("hello");
    return 0;
}
