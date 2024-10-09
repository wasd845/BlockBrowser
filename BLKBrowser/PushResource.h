#ifndef _PUSH_RESOURCE_H
#define _PUSH_RESOURCE_H
//////////////////////////////////////////////////////////////////////////////////////////////

#define LOCAL_RESOUCE HINSTANCE PushResource::s_hInstanceDefault=NULL
class PushResource
{
private:
    HINSTANCE m_hInstanceOld;
public:
    static HINSTANCE s_hInstanceDefault;
	static void SetDefaultResource(HINSTANCE hInstNew){s_hInstanceDefault=hInstNew;}
public:
    PushResource(HINSTANCE hInstNew);
    PushResource();
    virtual ~PushResource();

    void   CommonConstruction(HINSTANCE);
};
inline void PushResource::CommonConstruction(HINSTANCE hInst)
{
    m_hInstanceOld = AfxGetResourceHandle();
    AfxSetResourceHandle(hInst);
}
inline PushResource::PushResource(HINSTANCE hInstNew)
{
    CommonConstruction(hInstNew);
}
inline PushResource::PushResource()
{
    CommonConstruction(s_hInstanceDefault);
}
inline PushResource::~PushResource()
{
    AfxSetResourceHandle(m_hInstanceOld);
}

#endif