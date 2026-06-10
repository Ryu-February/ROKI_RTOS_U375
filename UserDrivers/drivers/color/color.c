///*
// * color.c
// *
// *  Created on: Sep 25, 2025
// *      Author: RCY
// */
//
//
//#include "color.h"
//#include "flash.h"
//#include "uart.h"
//#include "i2c.h"
//#include "mode_sw.h"
//
//
//// --- Ambiguity reject (튜닝 지점) ---
//#define REJ_L_MIN_CARD      700    // 너무 어두우면(들림/그림자) 거절: R+G+B < L_MIN
//#define REJ_L_MIN_LINE      200    // 너무 어두우면(들림/그림자) 거절: R+G+B < L_MIN
//#define REJ_CHROMA_MIN      400    // 무채색/바닥: max(R,G,B)-min(R,G,B) < 이 값이면 거절
//#define REJ_CHROMA_REL_MIN  0.06f  // 상대 채도 최소값
//
//// best/second 기준(낮을수록 좋은 distance)
//#define REJ_RATIO_MAX      0.5f   // best/second > 0.82 ⇒ 두 후보가 너무 비슷 → 거절
//
//#define REJ_ORG_RATIO_MAX  0.2f   // best/second > 0.82 ⇒ 두 후보가 너무 비슷 → 거절
//#define REJ_YEL_RATIO_MAX  0.2f   // best/second > 0.82 ⇒ 두 후보가 너무 비슷 → 거절
//#define REJ_GRN_RATIO_MAX  0.4f   // best/second > 0.82 ⇒ 두 후보가 너무 비슷 → 거절
//#define REJ_REL_MIN        0.05f   // (second-best)/second 와의 상대 마진 < 10% → 거절
//                                   // == (second-best)/second = 1 - ratio
//
//reference_entry_t color_reference_tbl_left[COLOR_COUNT];
//reference_entry_t color_reference_tbl_right[COLOR_COUNT];
//
//uint8_t  offset_side;
//uint16_t offset_black;
//uint16_t offset_white;
//uint16_t offset_average;
//
//color_mode_t insert_queue[MAX_INSERTED_COMMANDS];
//uint8_t insert_index = 0;
//
//extern volatile uint8_t  offset_side_local;   // 0: RIGHT, 1: LEFT (프로젝트 정의에 맞게 사용)
//extern volatile uint16_t offset_avg_local;
//
///* --------- Low-level BH1749 --------- */
//
//void bh1749_write_reg(uint8_t dev_addr, uint8_t reg, uint8_t data)
//{
//    i2c_write(dev_addr, reg, data);
//}
//
//uint8_t bh1749_read_u8(uint8_t dev_addr, uint8_t reg)
//{
//    return i2c_read(dev_addr, reg);
//}
//
//uint16_t bh1749_read_u16(uint8_t dev_addr, uint8_t lsb_reg)
//{
//    uint8_t lsb = i2c_read(dev_addr, lsb_reg);
//    uint8_t msb = i2c_read(dev_addr, lsb_reg + 1);
//    return (uint16_t)((msb << 8) | lsb);
//}
//
//void bh1749_init(uint8_t dev_addr, uint8_t rgb_gain, uint8_t ir_gain, uint8_t meas_mode)
//{
//    // 1) Software Reset
//    bh1749_write_reg(dev_addr, BH1749_REG_SYSTEM_CONTROL, BH1749_SW_RESET);
//    delay_ms(50);
//
//    // 2) MODE_CONTROL1: [IR_GAIN][RGB_GAIN][MEAS_MODE]
//    // 비트 배치는 데이터시트 정의에 따름. 기본값: x1/x1, 120ms
//    uint8_t mode1 =
//        ((ir_gain  & 0x03u) << 6) |
//        ((rgb_gain & 0x03u) << 4) |
//        (meas_mode & 0x07u);
//    bh1749_write_reg(dev_addr, BH1749_REG_MODE_CTRL1, mode1);
//
//
//    // 3) MODE_CONTROL2: RGB_EN=1 (측정 시작)
//    bh1749_write_reg(dev_addr, BH1749_REG_MODE_CTRL2, BH1749_RGB_EN);
//
////    uint8_t data = i2c_read(BH1749_ADDR_LEFT, BH1749_REG_MODE_CTRL2);
//    // (옵션) 유효 데이터 대기 (VALID 폴링)
//    for (int i = 0; i < 5; i++)
//    {
//        uint8_t v = bh1749_read_u8(dev_addr, BH1749_REG_MODE_CTRL2);
//        if (v & BH1749_VALID)
//        {
//            break;
//        }
//        delay_ms(5);
//    }
//}
//
///* --------- High-level Color --------- */
//
//void color_init(void)
//{
//    // 기본: x1/x1, 120ms
//    bh1749_init(BH1749_ADDR_LEFT,  BH1749_GAIN_X1,  BH1749_GAIN_X1,  BH1749_MEAS_35MS);
//    bh1749_init(BH1749_ADDR_RIGHT, BH1749_GAIN_X1,  BH1749_GAIN_X1,  BH1749_MEAS_35MS);
//}
//
//bh1749_color_data_t bh1749_read_rgbir(uint8_t dev_addr)
//{
//    bh1749_color_data_t c;
//
//    c.red   = bh1749_read_u16(dev_addr, BH1749_REG_RED_LSB);
//    c.green = bh1749_read_u16(dev_addr, BH1749_REG_GREEN_LSB);
//    c.blue  = bh1749_read_u16(dev_addr, BH1749_REG_BLUE_LSB);
//    c.ir    = bh1749_read_u16(dev_addr, BH1749_REG_IR_LSB);
//
//    return c;
//}
//
//void save_color_reference(uint8_t sensor_side, color_t color, uint16_t r, uint16_t g, uint16_t b)
//{
//    rgb_raw_t raw = { .red_raw = r, .green_raw = g, .blue_raw = b };
//    uint64_t  offset = calculate_brightness(r, g, b);
//
//    reference_entry_t entry = { .raw = raw, .color = color, .offset = offset };
//
//    if (sensor_side == BH1749_ADDR_LEFT)
//    {
//        color_reference_tbl_left[color] = entry;
//    }
//    else
//    {
//        color_reference_tbl_right[color] = entry;
//    }
//
//    // Flash 저장 (플랫폼 함수)
//    flash_write_color_reference(sensor_side, color, entry);
//}
//
//color_t classify_color(uint8_t left_right, uint16_t r, uint16_t g, uint16_t b, uint16_t ir)
//{
//    (void)ir; // 원하면 glare 억제용으로 사용 가능
//
//    // 0) 밝기/무채색 지표 계산(사전계산만, 리젝은 사후에 애매할 때만 적용)
//    uint32_t L = (uint32_t)r + (uint32_t)g + (uint32_t)b;
//    uint16_t maxc = r; if (g>maxc) maxc=g; if (b>maxc) maxc=b;
//    uint16_t minc = r; if (g<minc) minc=g; if (b<minc) minc=b;
//
//    const reference_entry_t* table = (left_right == BH1749_ADDR_LEFT)
//                                     ? color_reference_tbl_left
//                                     : color_reference_tbl_right;
//
//    // 1) 최인접 + second-best 같이 구함
//    float best = 1e30f, second = 1e30f;
//    color_t best_match = COLOR_GRAY;
//
//    for (int i = 0; i < COLOR_COUNT; i++)
//    {
//        float R0 = (float)table[i].raw.red_raw;
//        float G0 = (float)table[i].raw.green_raw;
//        float B0 = (float)table[i].raw.blue_raw;
//        float denom = R0 * R0 + G0 * G0 + B0 * B0 + 1e-9f;//분모에 들어가는데 1e-9f는 10^-9임 분모가 0이 안 되게만
//        float k = ((float)r * R0 + (float)g * G0 + (float)b * B0) / denom;//발기 비율차이 보정
//        float dr = (float)r - k * R0;//색상이 같으면 여기서 0에 가깝게 값이 떨어짐
//        float dg = (float)g - k * G0;
//        float db = (float)b - k * B0;
//        float dist = dr * dr + dg * dg + db * db;//유클리드 거리 측정
//
//        if (dist < best)
//        {
//            second = best;
//            best = dist;
//            best_match = table[i].color;
//        }
//        else if (dist < second)
//        {
//            second = dist;
//        }
//    }
//
//    // 2) 애매하면 거절 (라티오 + 상대마진)
//    //  - Lowe’s ratio와 유사: best/second가 1에 가까우면 애매
//    //  - 상대 마진: (second-best)/second (0~1)
//    if (second < 1e-9f)//second가 0에 가까울 확률이 거의 없음 10^(-9)
//    {
//        // 보호절: 이론상 second==0은 거의 없음. 그냥 통과.
//    }
//    else
//    {
//        float ratio   = best / second;                 // 0(선명) ~ 1(애매)
//        float relmag  = (second - best) / second;      // 0(애매) ~ 1(선명)
//
//        float ratio_thresh = (best_match == COLOR_ORANGE) ? REJ_ORG_RATIO_MAX
//                             : (best_match == COLOR_YELLOW) ? REJ_YEL_RATIO_MAX
//							 : (best_match == COLOR_GREEN) ? REJ_GRN_RATIO_MAX
//                             : REJ_RATIO_MAX;
//
//        if (ratio > ratio_thresh || relmag < REJ_REL_MIN)
//        {
//            // 애매한 경우에만 밝기/채도 게이트 적용해 보수적으로 GRAY 처리
//        	if (L < ((mode_sw_get() == MODE_CARD) ? REJ_L_MIN_CARD : REJ_L_MIN_LINE) || (float)(maxc - minc) < REJ_CHROMA_REL_MIN * (float)L)
//            {
//                return COLOR_GRAY;
//            }
//        }
//    }
//
//    return best_match;
//}
//
//uint8_t classify_color_side(uint8_t color_side)
//{
//    uint8_t addr = color_side;
//
//    bh1749_color_data_t c = bh1749_read_rgbir(addr);
//    color_t detected = classify_color(addr, c.red, c.green, c.blue, c.ir);
//
//    return (uint8_t)detected;
//}
//
//const char* color_to_string(color_t color)
//{
//    static const char* color_names[] =
//    {
//		"GRAY",
//        "RED",
//        "ORANGE",
//        "YELLOW",
//        "GREEN",
//        "BLUE",
//        "PURPLE",
//        "LIGHT_GREEN",
//        "SKY_BLUE",
//        "PINK",
//        "BLACK",
//        "WHITE",
//    };
//
//    if (color < 0 || color >= COLOR_COUNT)
//    {
//        return "UNKNOWN";
//    }
//
//    return color_names[color];
//}
//
//void load_color_reference_table(void)
//{
//    for (int i = 0; i < COLOR_COUNT; i++)
//    {
//        color_reference_tbl_left[i]  = flash_read_color_reference(BH1749_ADDR_LEFT,  i);
//        color_reference_tbl_right[i] = flash_read_color_reference(BH1749_ADDR_RIGHT, i);
//    }
//}
//
//void debug_print_color_reference_table(void)
//{
//    uart_printf("=== LEFT COLOR REFERENCE TABLE ===\r\n");
//    for (int i = 0; i < COLOR_COUNT; i++)
//    {
//        reference_entry_t e = color_reference_tbl_left[i];
//        uart_printf("[%2d | %-11s] R: %4d, G: %4d, B: %4d, OFFSET: %8llu\r\n",
//                    i, color_to_string(e.color),
//                    e.raw.red_raw, e.raw.green_raw, e.raw.blue_raw, e.offset);
//    }
//
//    uart_printf("=== RIGHT COLOR REFERENCE TABLE ===\r\n");
//    for (int i = 0; i < COLOR_COUNT; i++)
//    {
//        reference_entry_t e = color_reference_tbl_right[i];
//        uart_printf("[%2d | %-11s] R: %4d, G: %4d, B: %4d, OFFSET: %8llu\r\n",
//                    i, color_to_string(e.color),
//                    e.raw.red_raw, e.raw.green_raw, e.raw.blue_raw, e.offset);
//    }
//
//    uart_printf("=== BRIGHTNESS OFFSET TABLE ===\r\n");
//    uart_printf("offset_black: %d | offset_white: %d\r\n", offset_black, offset_white);
//    uart_printf("offset_aver: %d\r\n", offset_average);
//
//    offset_avg_local = offset_average;
//	offset_side_local = offset_side;
//}
//
//uint32_t calculate_brightness(uint16_t r, uint16_t g, uint16_t b)
//{
//    // H/W 없이 빠른 정수계수 Y ≈ 0.2126R + 0.7152G + 0.0722B
//    return (uint32_t)((218u * r + 732u * g + 74u * b) >> 10);
//}
//
//void calculate_color_brightness_offset(void)
//{
//    offset_black = (uint16_t)abs((int)color_reference_tbl_left[COLOR_BLACK].offset
//                               - (int)color_reference_tbl_right[COLOR_BLACK].offset);
//    offset_white = (uint16_t)abs((int)color_reference_tbl_left[COLOR_WHITE].offset
//                               - (int)color_reference_tbl_right[COLOR_WHITE].offset);
//
//    offset_side = (color_reference_tbl_left[COLOR_BLACK].offset >
//                   color_reference_tbl_right[COLOR_BLACK].offset) ? LEFT : RIGHT;
//
//    offset_average = (uint16_t)((offset_black + offset_white) / 2);
//}
//
//color_mode_t color_to_mode(color_t color)
//{
//    switch (color)
//    {
//        case COLOR_RED:         return MODE_BACKWARD;
//        case COLOR_YELLOW:      return MODE_RIGHT;
//        case COLOR_GREEN:       return MODE_FORWARD;
//        case COLOR_BLUE:		return MODE_LEFT;
//        case COLOR_PURPLE:		return MODE_REPEAT_ONCE;
//        case COLOR_PINK:		return MODE_REPEAT_TWICE;
//        case COLOR_SKY_BLUE:	return MODE_REPEAT_THIRD;
//        default:                return MODE_NONE;
//    }
//}
