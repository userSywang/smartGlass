#include <cassert>
#include <cstring>

#include "LensDataProvider.hpp"

int main()
{
    const LensDataProvider &provider = simulationLensDataProvider();

    assert(provider.notifications().size == 3);
    assert(provider.notifications()[0].channel == NotificationChannel::WeChat);
    assert(std::strcmp(provider.notifications()[2].source, "钉钉") == 0);

    assert(provider.translations().size == 3);
    assert(provider.realtimeTranslations().size == 4);
    assert(provider.bilingualMineSamples().size == 3);
    assert(provider.bilingualOtherSamples().size == 3);
    assert(provider.musicTracks().size == 3);
    assert(provider.prompterSegments().size == 8);
    assert(provider.meetingCards().size == 3);
    assert(provider.meetingTranscriptLines().size == 4);

    return 0;
}
