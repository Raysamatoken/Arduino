#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ D2, /* data=*/ D1, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

#define FONT u8g2_font_wqy14_t_gb2312b

//#define FONT u8g2_font_wqy16_t_chinese1
//#define FONT u8g2_font_wqy16_t_gb2312b

// The next two macros define the scroll speed of the short story
#define SCROLL_DELTA 2
#define SCROLL_DELAY 200


const char c_str[] = 
"群主是猪\n"
"晓烟是美少女\n"
"处女座是洗碗机\n";

char buf[48];            // there are at most 8 chinese glyphs per line, max buf size is 8*3 = 24 
 
uint8_t total_lines;        // the total number of lines in the story
uint8_t i;                // loop variable for the lines
uint8_t line_cnt;        // number of lines to draw, usually equal to lines_per_draw
uint8_t start_line;        // topmost visible line, derived from top_window_pos
uint8_t lines_per_draw;    // how many lines to draw on the screen, derived from font and display height
uint16_t glyph_height;    // height of the glyphs

uint16_t top_window_pos;        // defines the display position in pixel within the text
uint16_t total_height;            // total height in pixel, derived from font height and total_lines
u8g2_uint_t top_offset;            // offset between the first visible line and the display


void setup(void) {

  /* U8g2 Project: SSD1306 Test Board */
  pinMode(D2, OUTPUT);
  pinMode(D1, OUTPUT);
  digitalWrite(10, 0);
  digitalWrite(9, 0);        

  /* U8g2 Project: T6963 Test Board */
  //pinMode(18, OUTPUT);
  //digitalWrite(18, 1);    

  /* U8g2 Project: KS0108 Test Board */
  //pinMode(16, OUTPUT);
  //digitalWrite(16, 0);    

  /* U8g2 Project: LC7981 Test Board, connect RW to GND */
  //pinMode(17, OUTPUT);
  //digitalWrite(17, 0);    

  /* U8g2 Project: Pax Instruments Shield: Enable Backlight */
  //pinMode(6, OUTPUT);
  //digitalWrite(6, 0);    

  u8g2.begin();  
  

  /* select a font */
  
 // u8g2.setFont(u8g2_font_wqy12_t_chinese1);    // two unknown glyphs
 //u8g2.setFont(u8g2_font_wqy12_t_chinese3);    // two unknown glyphs

  //u8g2.setFont(u8g2_font_wqy12_t_gb2312a);    // ";" is missing
  //u8g2.setFont(u8g2_font_wqy12_t_gb2312b);    // all glyphs available

  u8g2.setFont(FONT);
  
  /* calculate the length of the text in lines */
  total_lines = u8x8_GetStringLineCnt(c_str);
  
  /* get the height of the glyphs */
  glyph_height = u8g2.getMaxCharHeight();

  /* calculate the height of the text in pixel */
  total_height = (uint16_t)total_lines * (uint16_t)glyph_height;
  

  /* calculate how many lines must be drawn on the screen */
  lines_per_draw = u8g2.getDisplayHeight() / glyph_height;
  lines_per_draw += 2;

  /* start at the top of the text */
  top_window_pos = 0;

}

void loop(void) {

  start_line = top_window_pos / glyph_height;
  top_offset = top_window_pos % glyph_height;
  
  line_cnt = total_lines - start_line;
  if ( line_cnt > lines_per_draw )
    line_cnt = lines_per_draw;

  u8g2.firstPage();
  do {
      for( i = 0; i < line_cnt; i++ )
      {
    /* copy a line of the text to the local buffer */
    u8x8_CopyStringLine(buf, i+start_line, c_str);
    
    /* draw the content of the local buffer */
    u8g2.drawUTF8(0, i*glyph_height-top_offset +glyph_height, buf);
      }
  } while ( u8g2.nextPage() );
  
  delay(SCROLL_DELAY);
  top_window_pos += SCROLL_DELTA;
}
