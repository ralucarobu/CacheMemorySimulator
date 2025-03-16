#pragma once
#ifndef WRITEPOLICY_H
#define WRITEPOLICY_H

class WritePolicy {
private:
    int policy; // 0 = Write Back, 1 = Write Through

public:
    WritePolicy() : policy(0) {} // implicit punem 0 

    void setPolicy(int p);
    int getPolicy() const;
};

#endif
