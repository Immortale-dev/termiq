#ifndef _TQ_TERM_UNICODE_DRAWS
#define _TQ_TERM_UNICODE_DRAWS

#include <string>

namespace termiq {
	namespace draw_chars {
		using draw_char_t = std::basic_string<unsigned char>;
		inline constexpr draw_char_t C_H{0xE2, 0x94, 0x80};
		inline constexpr draw_char_t C_V{0xE2, 0x94, 0x82};
		inline constexpr draw_char_t C_X{0xE2, 0x94, 0xBC};
		inline constexpr draw_char_t C_HT{0xE2, 0x94, 0xB4};
		inline constexpr draw_char_t C_HB{0xE2, 0x94, 0xAC};
		inline constexpr draw_char_t C_VL{0xE2, 0x94, 0xA4};
		inline constexpr draw_char_t C_VR{0xE2, 0x94, 0x9C};
		inline constexpr draw_char_t C_TL{0xE2, 0x94, 0x98};
		inline constexpr draw_char_t C_TR{0xE2, 0x94, 0x94};
		inline constexpr draw_char_t C_BL{0xE2, 0x94, 0x90};
		inline constexpr draw_char_t C_BR{0xE2, 0x94, 0x8C};

		inline constexpr draw_char_t C_H_BOLD{0xE2, 0x94, 0x81};
		inline constexpr draw_char_t C_V_BOLD{0xE2, 0x94, 0x83};
		inline constexpr draw_char_t C_X_BOLD{0xE2, 0x95, 0x8B};
		inline constexpr draw_char_t C_HT_BOLD{0xE2, 0x94, 0xBB};
		inline constexpr draw_char_t C_HB_BOLD{0xE2, 0x94, 0xB3};
		inline constexpr draw_char_t C_VL_BOLD{0xE2, 0x94, 0xAB};
		inline constexpr draw_char_t C_VR_BOLD{0xE2, 0x94, 0xA3};
		inline constexpr draw_char_t C_TL_BOLD{0xE2, 0x94, 0x9B};
		inline constexpr draw_char_t C_TR_BOLD{0xE2, 0x94, 0x97};
		inline constexpr draw_char_t C_BL_BOLD{0xE2, 0x94, 0x93};
		inline constexpr draw_char_t C_BR_BOLD{0xE2, 0x94, 0x8F};

		inline constexpr draw_char_t C_H_DOUBLE{0xE2, 0x95, 0x90};
		inline constexpr draw_char_t C_V_DOUBLE{0xE2, 0x95, 0x91};
		inline constexpr draw_char_t C_X_DOUBLE{0xE2, 0x95, 0xAC};
		inline constexpr draw_char_t C_HT_DOUBLE{0xE2, 0x95, 0xA9};
		inline constexpr draw_char_t C_HB_DOUBLE{0xE2, 0x95, 0xA6};
		inline constexpr draw_char_t C_VL_DOUBLE{0xE2, 0x95, 0xA3};
		inline constexpr draw_char_t C_VR_DOUBLE{0xE2, 0x95, 0xA0};
		inline constexpr draw_char_t C_TL_DOUBLE{0xE2, 0x95, 0x9D};
		inline constexpr draw_char_t C_TR_DOUBLE{0xE2, 0x95, 0x9A};
		inline constexpr draw_char_t C_BL_DOUBLE{0xE2, 0x95, 0x97};
		inline constexpr draw_char_t C_BR_DOUBLE{0xE2, 0x95, 0x94};

		inline constexpr draw_char_t C_H_GAP{0xE2, 0x95, 0x8C};
		inline constexpr draw_char_t C_V_GAP{0xE2, 0x95, 0x8E};
		inline constexpr draw_char_t C_H_GAP_BOLD{0xE2, 0x95, 0x8D};
		inline constexpr draw_char_t C_V_GAP_BOLD{0xE2, 0x95, 0x8F};
		inline constexpr draw_char_t C_H_DASHED{0xE2, 0x94, 0x84};
		inline constexpr draw_char_t C_V_DASHED{0xE2, 0x94, 0x86};
		inline constexpr draw_char_t C_H_DASHED_BOLD{0xE2, 0x94, 0x85};
		inline constexpr draw_char_t C_V_DASHED_BOLD{0xE2, 0x94, 0x87};
		inline constexpr draw_char_t C_H_DOTTED{0xE2, 0x94, 0x88};
		inline constexpr draw_char_t C_V_DOTTED{0xE2, 0x94, 0x8A};
		inline constexpr draw_char_t C_H_DOTTED_BOLD{0xE2, 0x94, 0x89};
		inline constexpr draw_char_t C_V_DOTTED_BOLD{0xE2, 0x94, 0x8B};

		inline constexpr draw_char_t C_TL_ROUND{0xE2, 0x95, 0xAF};
		inline constexpr draw_char_t C_TR_ROUND{0xE2, 0x95, 0xB0};
		inline constexpr draw_char_t C_BL_ROUND{0xE2, 0x95, 0xAE};
		inline constexpr draw_char_t C_BR_ROUND{0xE2, 0x95, 0xAD};
	}
}

#endif // _TQ_TERM_UNICODE_DRAWS
