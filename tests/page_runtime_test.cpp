#include <cassert>

#include "PageRuntime.hpp"

int main()
{
    assert(pageTimerForApp(-1) == PageTimerKind::None);
    assert(pageTimerForApp(0) == PageTimerKind::None);
    assert(pageTimerForApp(1) == PageTimerKind::Camera);
    assert(pageTimerForApp(2) == PageTimerKind::Navigation);
    assert(pageTimerForApp(3) == PageTimerKind::Notification);
    assert(pageTimerForApp(4) == PageTimerKind::Music);
    assert(pageTimerForApp(5) == PageTimerKind::Prompter);
    assert(pageTimerForApp(6) == PageTimerKind::Translate);
    assert(pageTimerForApp(7) == PageTimerKind::RealtimeTranslate);
    assert(pageTimerForApp(8) == PageTimerKind::BilingualChat);
    assert(pageTimerForApp(9) == PageTimerKind::Meeting);
    assert(pageTimerForApp(10) == PageTimerKind::None);
    assert(pageTimerForApp(11) == PageTimerKind::None);
    assert(pageTimerForApp(12) == PageTimerKind::None);
    return 0;
}
