#ifndef ENABLE_CONTROLLER_H
#define ENABLE_CONTROLLER_H

class EnableController
{
public:
    EnableController();
    bool getIsEnabled();
    void setIsEnabled(bool value);

private:
    bool m_isEnabled;
};

#endif