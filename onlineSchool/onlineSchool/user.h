#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    User(const std::string& email) : email(email) {}
    std::string getEmail() const { return email; }
private:
    std::string email;
};

#endif // USER_H