#include "syati.h";
#include "nw4r.h"

class TextWriter : public NameObj {
public:
    TextWriter(const char* pName);
    virtual ~TextWriter();
    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;
};