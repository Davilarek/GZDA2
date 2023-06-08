//
// Created by Emile on 18/04/2017.
//
#include "UI_Controls.h"
#include "UI_Window.h"
#include "OpenGLUtils.h"

using namespace touchcontrols;

UI_Controls::UI_Controls( std::string t ):
tag(t)
{
    yOffset = 0;
    scrolling = false;
}

void UI_Controls::setEnabled(bool v)
{
    enabled = v;
}

bool UI_Controls::getEnabled()
{
    return enabled;
}


void UI_Controls::addControl(ControlSuper *cntrl)
{
	controls.push_back((cntrl));
}

bool UI_Controls::processPointer(int action, int pid, float x, float y)
{
    int size = controls.size();

    bool eventUsed = false;
    for (int n=0;n<size;n++)
    {
        ControlSuper *cs = controls.at(size - 1 - n); // Start from top of stack
        if (cs->isEnabled())
        {
            float ys;
            ys = (cs->type == TC_TYPE_UI_WINDOW) ? y : y + yOffset; //Dont apply scroll offset to window

            if( cs->processPointer(action,pid, x, ys ))
            {
                eventUsed = true;
            }
        }
    }

    if (action == P_UP)
    {
         scrolling = false;
    }

    if( !eventUsed ) // Finger not over a usable control
    {
        if (action == P_DOWN)
        {
            finger1.x = x;
            finger1.y = y;
            scrolling = true;
        }
        else if(action == P_MOVE) //Scroll controls
        {
            if( scrolling )
            {
                yOffset += finger1.y - y;

                if( yOffset < 0 )
                    yOffset = 0;

                finger1.x = x;
                finger1.y = y;
            }
        }
    }


    return true;
}

int UI_Controls::draw ()
{
    glColor4f(1, 1, 1, 1);

    int size = controls.size();
    for (int n=0;n<size;n++) //draw
    {
        ControlSuper *c = controls.at(n);
        if (c->isEnabled())
        {
            glLoadIdentity();

            glScalef(GLScaleWidth, GLScaleHeight, 1);

            // Scroll, but don't scroll the background window
            if( c->type != TC_TYPE_UI_WINDOW )
            {
                glTranslatef( 0, yOffset, 0 );
            }
            c->drawGL();
        }
    }
    // The UI window enables this
    glDisable( GL_SCISSOR_TEST );

    return 0;
}

void UI_Controls::initGL ()
{
	int size = controls.size();
	for (int n=0;n<size;n++) //draw
	{
		ControlSuper *c = controls.at(size-1-n);
		c->initGL();
	}
}

void UI_Controls::setAlpha(float a)
{

}

void UI_Controls::fade(fadedir_t dir,int steps)
{

}