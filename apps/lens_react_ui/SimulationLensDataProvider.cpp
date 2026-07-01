#include "LensDataProvider.hpp"

namespace {
template <typename T, std::size_t N>
constexpr LensDataView<T> viewOf(const T (&items)[N])
{
    return {items, N};
}

constexpr NotificationSample kNotifications[] = {
    {NotificationChannel::WeChat, "微信", "张三", "晚饭定在科技园那家日料店了。"},
    {NotificationChannel::Sms, "短信", "中国移动", "验证码 482916，5 分钟内有效。"},
    {NotificationChannel::DingTalk, "钉钉", "项目会议", "15:30 产品评审会即将开始。"},
};

constexpr TranslationSample kTranslations[] = {
    {"请问去最近的地铁站怎么走？", "Excuse me, how do I get to the nearest subway station?"},
    {"这附近有推荐的咖啡店吗？", "Are there any recommended coffee shops nearby?"},
    {"麻烦帮我结账，可以用信用卡吗？", "Check, please. Can I pay by credit card?"},
};

constexpr TranslationSample kRealtimeTranslations[] = {
    {"We need to memorize these basic structures and their directing effects.",
     "我们需要记住这些基本结构及其导向效应。"},
    {"So that we can better predict and design organic reactions.",
     "这样我们才能更好地预测和设计有机反应。"},
    {"Are there any questions about this part of the lecture?",
     "关于这部分讲座有什么问题吗？"},
    {"The next example shows how the reaction changes under different conditions.",
     "下一个例子展示了反应在不同条件下的变化。"},
};

constexpr BilingualSample kBilingualMineSamples[] = {
    {"我说（中文）", "你好，很高兴认识你。", "Hello, nice to meet you.", true},
    {"我说（中文）", "请问去最近的地铁站怎么走？",
     "Excuse me, how do I get to the nearest subway station?", true},
    {"我说（中文）", "我们可以在门口集合。", "We can meet at the entrance.", true},
};

constexpr BilingualSample kBilingualOtherSamples[] = {
    {"对方（English）", "Go straight and turn left at the corner.", "直走并在拐角处左转。", false},
    {"对方（English）", "The meeting starts in ten minutes.", "会议将在十分钟后开始。", false},
    {"对方（English）", "I can help you order coffee.", "我可以帮你点咖啡。", false},
};

constexpr MusicTrackSample kMusicTracks[] = {
    {"Cyberpunk City", "Synthwave Maker", 222, 0xc04bf5, 0x18b7d5},
    {"Night Drive", "Neon Pulse", 196, 0x3660f6, 0x34d399},
    {"Glass Horizon", "Ambient Lab", 248, 0xf473b9, 0xf59e0b},
};

constexpr const char *kPrompterSegments[] = {
    "大家好，欢迎来到新品发布会。",
    "我们的 AR 眼镜采用了全新的设计，",
    " Our AR glasses feature a broader design，",
    " 支持全球超过 100 种语言的实时翻译，",
    " and support translation for over 100 languages. ",
    "准确率高达 98%，",
    "做到真正意义上的无障碍沟通。",
    "感谢大家的关注与支持。",
};

constexpr MeetingCardSample kMeetingCards[] = {
    {MeetingCardKind::Todo, "待办事项", "前端参与后端联调登录页接口"},
    {MeetingCardKind::Schedule, "排期节点", "周三前：登录页必须上线"},
    {MeetingCardKind::KeyInfo, "关键信息", "周五下午：交付AR新版视觉初稿"},
};

constexpr const char *kMeetingTranscriptLines[] = {
    "大家上午好，现在开始同步项目进度。",
    "前端的登录页和侧边栏都完成了没有？",
    "基本完成了，还需要和后端联调接口。",
    "预计周五下午会初稿，重点优化了暗色模式的对比度。",
};

class SimulationLensDataProvider final : public LensDataProvider {
public:
    LensDataView<NotificationSample> notifications() const override { return viewOf(kNotifications); }
    LensDataView<TranslationSample> translations() const override { return viewOf(kTranslations); }
    LensDataView<TranslationSample> realtimeTranslations() const override { return viewOf(kRealtimeTranslations); }
    LensDataView<BilingualSample> bilingualMineSamples() const override { return viewOf(kBilingualMineSamples); }
    LensDataView<BilingualSample> bilingualOtherSamples() const override { return viewOf(kBilingualOtherSamples); }
    LensDataView<MusicTrackSample> musicTracks() const override { return viewOf(kMusicTracks); }
    LensDataView<const char *> prompterSegments() const override { return viewOf(kPrompterSegments); }
    LensDataView<MeetingCardSample> meetingCards() const override { return viewOf(kMeetingCards); }
    LensDataView<const char *> meetingTranscriptLines() const override { return viewOf(kMeetingTranscriptLines); }
};
}

const LensDataProvider &simulationLensDataProvider()
{
    static const SimulationLensDataProvider provider;
    return provider;
}
