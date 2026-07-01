#pragma once

#include <cstddef>
#include <cstdint>

template <typename T>
struct LensDataView {
    // Providers retain ownership; returned items must remain valid for the provider lifetime.
    const T *items = nullptr;
    std::size_t size = 0;

    const T &operator[](std::size_t index) const
    {
        return items[index];
    }
};

enum class NotificationChannel : uint8_t {
    WeChat,
    Sms,
    DingTalk,
};

struct NotificationSample {
    NotificationChannel channel;
    const char *source;
    const char *sender;
    const char *message;
};

struct TranslationSample {
    const char *source;
    const char *translation;
};

struct BilingualSample {
    const char *role;
    const char *primary;
    const char *secondary;
    bool mine;
};

struct MusicTrackSample {
    const char *title;
    const char *artist;
    uint16_t duration_seconds;
    uint32_t cover_start_rgb;
    uint32_t cover_end_rgb;
};

enum class MeetingCardKind : uint8_t {
    Todo,
    Schedule,
    KeyInfo,
};

struct MeetingCardSample {
    MeetingCardKind kind;
    const char *title;
    const char *body;
};

class LensDataProvider {
public:
    virtual ~LensDataProvider() = default;

    virtual LensDataView<NotificationSample> notifications() const = 0;
    virtual LensDataView<TranslationSample> translations() const = 0;
    virtual LensDataView<TranslationSample> realtimeTranslations() const = 0;
    virtual LensDataView<BilingualSample> bilingualMineSamples() const = 0;
    virtual LensDataView<BilingualSample> bilingualOtherSamples() const = 0;
    virtual LensDataView<MusicTrackSample> musicTracks() const = 0;
    virtual LensDataView<const char *> prompterSegments() const = 0;
    virtual LensDataView<MeetingCardSample> meetingCards() const = 0;
    virtual LensDataView<const char *> meetingTranscriptLines() const = 0;
};

const LensDataProvider &simulationLensDataProvider();
