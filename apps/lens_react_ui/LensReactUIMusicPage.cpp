#include "LensUIInternal.hpp"

using namespace lens_ui;

void LensReactUI::createMusicPage(void)
{
    const bool compact = _view_height <= kCompactViewMaxHeight;
    const lv_coord_t cover_size = compact ? 96 : 148;
    const lv_coord_t cover_y = compact ? 54 : 96;
    const lv_coord_t title_y = compact ? 160 : 272;
    const lv_coord_t progress_y = compact ? 226 : 368;
    const lv_coord_t controls_y = compact ? 270 : 432;

    _music_cover = box(_page_content, cover_size, cover_size, kPurple, LV_OPA_COVER, compact ? 18 : 22);
    lv_obj_align(_music_cover, LV_ALIGN_TOP_MID, 0, cover_y);
    lv_obj_set_style_bg_grad_color(_music_cover, kCyan, 0);
    lv_obj_set_style_bg_grad_dir(_music_cover, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_shadow_width(_music_cover, compact ? 14 : 22, 0);
    lv_obj_set_style_shadow_color(_music_cover, LV_COLOR_MAKE(0x03, 0x10, 0x18), 0);
    lv_obj_set_style_shadow_opa(_music_cover, LV_OPA_50, 0);
    lv_obj_t *music = label(_music_cover, LV_SYMBOL_AUDIO, compact ? &lv_font_montserrat_34 : &lv_font_montserrat_48,
                            LV_COLOR_MAKE(0xd8, 0xe5, 0xff));
    lv_obj_center(music);

    _music_title = label(_page_content, "", compact ? &lv_font_montserrat_24 : &lv_font_montserrat_28, kText);
    lv_obj_set_width(_music_title, _view_width - 120);
    lv_label_set_long_mode(_music_title, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(_music_title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(_music_title, LV_ALIGN_TOP_MID, 0, title_y);

    _music_artist = label(_page_content, "", compact ? &lv_font_montserrat_14 : &lv_font_montserrat_16, kTextDim);
    lv_obj_set_width(_music_artist, _view_width - 120);
    lv_label_set_long_mode(_music_artist, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(_music_artist, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(_music_artist, LV_ALIGN_TOP_MID, 0, title_y + (compact ? 30 : 38));

    const lv_coord_t progress_width = compact ? 230 : 256;
    lv_obj_t *progress_track = box(_page_content, progress_width, 5, LV_COLOR_MAKE(0x2c, 0x2c, 0x2f), LV_OPA_COVER, 3);
    lv_obj_align(progress_track, LV_ALIGN_TOP_MID, 0, progress_y);
    _music_progress_fill = box(progress_track, 1, 5, kText, LV_OPA_COVER, 3);
    lv_obj_align(_music_progress_fill, LV_ALIGN_LEFT_MID, 0, 0);

    _music_elapsed_label = label(_page_content, "", &lv_font_montserrat_12, kTextDim);
    lv_obj_set_width(_music_elapsed_label, 40);
    lv_obj_set_style_text_align(_music_elapsed_label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_align_to(_music_elapsed_label, progress_track, LV_ALIGN_OUT_LEFT_MID, -22, 0);
    _music_duration_label = label(_page_content, "", &lv_font_montserrat_12, kTextDim);
    lv_obj_set_width(_music_duration_label, 40);
    lv_obj_align_to(_music_duration_label, progress_track, LV_ALIGN_OUT_RIGHT_MID, 22, 0);

    const lv_coord_t side_offset = compact ? 74 : 88;
    const lv_coord_t side_size = compact ? 44 : 54;
    const lv_coord_t play_size = compact ? 58 : 66;
    lv_obj_t *prev_btn = box(_page_content, side_size, side_size, kBlack, LV_OPA_TRANSP, LV_RADIUS_CIRCLE);
    lv_obj_align(prev_btn, LV_ALIGN_TOP_MID, -side_offset, controls_y);
    lv_obj_add_flag(prev_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(prev_btn, reinterpret_cast<void *>(static_cast<uintptr_t>(0)));
    lv_obj_add_event_cb(prev_btn, onMusicControlClicked, LV_EVENT_CLICKED, this);
    lv_obj_t *prev = label(prev_btn, LV_SYMBOL_PREV, compact ? &lv_font_montserrat_24 : &lv_font_montserrat_28,
                           kTextDim);
    lv_obj_center(prev);
    lv_obj_clear_flag(prev, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *play_btn = box(_page_content, play_size, play_size, kText, LV_OPA_COVER, LV_RADIUS_CIRCLE);
    lv_obj_align(play_btn, LV_ALIGN_TOP_MID, 0, controls_y - ((play_size - side_size) / 2));
    lv_obj_add_flag(play_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(play_btn, reinterpret_cast<void *>(static_cast<uintptr_t>(1)));
    lv_obj_add_event_cb(play_btn, onMusicControlClicked, LV_EVENT_CLICKED, this);
    _music_play_icon = label(play_btn, LV_SYMBOL_PAUSE, compact ? &lv_font_montserrat_28 : &lv_font_montserrat_34,
                             kBlack);
    lv_obj_center(_music_play_icon);
    lv_obj_clear_flag(_music_play_icon, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *next_btn = box(_page_content, side_size, side_size, kBlack, LV_OPA_TRANSP, LV_RADIUS_CIRCLE);
    lv_obj_align(next_btn, LV_ALIGN_TOP_MID, side_offset, controls_y);
    lv_obj_add_flag(next_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(next_btn, reinterpret_cast<void *>(static_cast<uintptr_t>(2)));
    lv_obj_add_event_cb(next_btn, onMusicControlClicked, LV_EVENT_CLICKED, this);
    lv_obj_t *next = label(next_btn, LV_SYMBOL_NEXT, compact ? &lv_font_montserrat_24 : &lv_font_montserrat_28,
                           kTextDim);
    lv_obj_center(next);
    lv_obj_clear_flag(next, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *hint = cjk_label(_page_content, "‹ 按 ← → 键切歌，Enter 键播放/暂停 ›", kTextFaint);
    lv_obj_set_width(hint, _view_width - 80);
    lv_label_set_long_mode(hint, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(hint, LV_ALIGN_BOTTOM_MID, 0, compact ? -14 : -30);

    updateMusicPage();
}

void LensReactUI::updateMusicPage(void)
{
    if(_music_title == nullptr || _music_artist == nullptr || _music_elapsed_label == nullptr ||
       _music_duration_label == nullptr || _music_progress_fill == nullptr || _music_play_icon == nullptr) {
        return;
    }
    const MusicTrack &track = kMusicTracks[_music_track_index % kMusicTrackCount];
    if(_music_elapsed_seconds > track.duration_seconds) {
        _music_elapsed_seconds = track.duration_seconds;
    }

    lv_label_set_text(_music_title, track.title);
    lv_label_set_text(_music_artist, track.artist);
    if(_music_cover) {
        lv_obj_set_style_bg_color(_music_cover, track.cover_start, 0);
        lv_obj_set_style_bg_grad_color(_music_cover, track.cover_end, 0);
    }
    lv_label_set_text(_music_play_icon, _music_playing ? LV_SYMBOL_PAUSE : LV_SYMBOL_PLAY);

    char elapsed[8];
    char duration[8];
    format_time(elapsed, sizeof(elapsed), _music_elapsed_seconds);
    format_time(duration, sizeof(duration), track.duration_seconds);
    lv_label_set_text(_music_elapsed_label, elapsed);
    lv_label_set_text(_music_duration_label, duration);

    lv_obj_t *progress_track = lv_obj_get_parent(_music_progress_fill);
    const lv_coord_t track_width = progress_track ? lv_obj_get_width(progress_track) : 0;
    const lv_coord_t fill_width = track.duration_seconds == 0 ?
                                      1 :
                                      LV_MAX((lv_coord_t)1,
                                             (lv_coord_t)((track_width * _music_elapsed_seconds) /
                                                          track.duration_seconds));
    lv_obj_set_width(_music_progress_fill, fill_width);
    lv_obj_align(_music_progress_fill, LV_ALIGN_LEFT_MID, 0, 0);
}

void LensReactUI::toggleMusicPlayback(void)
{
    _music_playing = !_music_playing;
    updateMusicPage();
}

void LensReactUI::changeMusicTrack(int8_t delta)
{
    const int next = (static_cast<int>(_music_track_index) + static_cast<int>(delta) + kMusicTrackCount) %
                     kMusicTrackCount;
    _music_track_index = static_cast<uint8_t>(next);
    _music_elapsed_seconds = _music_track_index == 0 ? 72 : 0;
    _music_playing = true;
    updateMusicPage();
}
