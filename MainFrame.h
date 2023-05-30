#include <wx/frame.h>
#include <wx/display.h>
#include <wx/button.h>
#include <wx/choice.h>
#include "MainPanel.h"
#include "ReactionPanel.h"
#include "TheoryPanel.h"

class MainFrame:public wxFrame
{
public:
    MainFrame(const wxString &title);
private:
    MainPanel *mainPanel;
    ReactionPanel *reactionPanel;
    TheoryPanel *theoryPanel;
    wxChoice *choices;
    wxColour reactionColours[2][3];
    void ReactionChoice(wxCommandEvent &event);
    void PressedTheory(wxCommandEvent &event);
    void ReturnToMainPanel(wxCommandEvent &event);
    void ResetReaction(wxCommandEvent &event);
    void CloseApp(wxCommandEvent &event);
};
