#pragma once
namespace GRAPHIC {
    class DIBPlane;
};
class BaseBoardView : public WINDOW::BaseBoardView
{
public:
    static void DO()
    {
        GET().Create();
    };
protected:
    static BaseBoardView& GET()
    {
        static BaseBoardView theBaseBoardView;
        return theBaseBoardView;
    };
    BaseBoardView() {};
    virtual ~BaseBoardView(void) {};
    virtual	void Draw(const GRAPHIC::DIBPlane& aDIBPlane);
    virtual	void Process(const unsigned char keys[]);
};
