#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/timer.h>
#include <wx/tokenzr.h>

struct Tube
{
    wxTimer *timer;
    wxColour colour;
    wxRect rect;
    wxString substances[2],plusSign;
    bool extra,inReaction;
    int equilibrium,timerCounter;
    float factor;
};
struct Jar
{
    wxColour colour;
    wxBrushStyle brush;
    wxRect rect;
    wxString substance;
    int equilibriumChange;
};
struct Pipette
{
    wxColour colour;
    wxBrushStyle brush;
    wxString substance;
    wxRect rect;
};
class ReactionPanel:public wxPanel
{
public:
    ReactionPanel(wxWindow *parent,const wxSize &size);
    void InitializeReaction(wxString reaction,wxColour reactionColours[]);
private:
    wxBitmap background;
    wxString substances[7],reactionEquation;
    wxColour targetColours[3];
    wxPoint mouseOffset,pipetteTip[4];
    wxRect hintBox;
    Tube tubes[4];
    Jar jars[3];
    Pipette pipette;
    bool clickedPipette,pipetteFilled;
    int equilibriumChange,aboveTube,aboveJar,substancePosition;
    float scaleX,scaleY;
    void DrawReactionPanel(wxPaintEvent &event);
    void MouseClick(wxMouseEvent &event);
    void MouseDoubleClick(wxMouseEvent &event);
    void MouseMove(wxMouseEvent &event);
    void MouseRelease(wxMouseEvent &event);
    void ColourTimer(wxTimerEvent &event);
};
float min(float x,float y);
