#include "ftnoir_tracker_joystick.h"
#include "facetracknoir/plugin-support.h"

static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
{
    auto self = ( TrackerControls* )pContext;

    self->guids.push_back(pdidInstance->guidInstance);
    self->ui.joylist->addItem(QString(pdidInstance->tszInstanceName));

    return DIENUM_CONTINUE;
}

TrackerControls::TrackerControls() : tracker(nullptr)
{
    ui.setupUi( this );

    // Connect Qt signals to member-functions
    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(doOK()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(doCancel()));

    tie_setting(s.axis_0, ui.comboBox);
    tie_setting(s.axis_1, ui.comboBox_2);
    tie_setting(s.axis_2, ui.comboBox_3);
    tie_setting(s.axis_3, ui.comboBox_4);
    tie_setting(s.axis_4, ui.comboBox_5);
    tie_setting(s.axis_5, ui.comboBox_6);

    {
        auto hr = CoInitialize( nullptr );
        LPDIRECTINPUT8 g_pDI = nullptr;

        if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,
                                             IID_IDirectInput8, ( VOID** )&g_pDI, NULL ) ) )
            goto fin;

        if( FAILED( hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL,
                                             EnumJoysticksCallback,
                                             this,
                                             DIEDFL_ATTACHEDONLY )))
            goto fin;

fin:
        if (g_pDI)
            g_pDI->Release();
    }
	
	tie_setting(s.joyid, ui.joylist);
}

void TrackerControls::doOK() {
    s.b->save();
    if (tracker)
        tracker->reload();
    this->close();
}

void TrackerControls::doCancel() {
    s.b->revert();
    this->close();
}

extern "C" FTNOIR_TRACKER_BASE_EXPORT ITrackerDialog* CALLING_CONVENTION GetDialog( )
{
    return new TrackerControls;
}
