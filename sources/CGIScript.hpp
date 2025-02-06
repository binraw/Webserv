#ifndef CGISCRIPT_HPP
# define CGISCRIPT_HPP

class CGIScript
{
private:
    
public:
    virtual void execute() = 0;
    virtual ~CGIScript(){};
};


class PHPScript : public CGIScript
{
public:
    void execute() override;
};

class PerlScript : public CGIScript
{
    public:
    void execute() override;
};


//possibiliter de rajouter ensuite une class par cgi (choisi) supporter par le server




#endif