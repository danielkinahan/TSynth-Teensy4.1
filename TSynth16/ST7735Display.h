#include "TeensyThreads.h"

#define sclk 27
#define mosi 26
#define cs 2
#define dc 3
#define rst 9
#define DISPLAYTIMEOUT 700

#include <Adafruit_GFX.h>
#include "ST7735_t3.h" // Local copy from TD1.48 that works for 0.96" IPS 160x80 display

#include <Fonts/Org_01.h>
#include "Yeysk16pt7b.h"
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansOblique24pt7b.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>

#define PULSE 1
#define VAR_TRI 2
#define FILTER_ENV 3
#define AMP_ENV 4

ST7735_t3 tft = ST7735_t3(cs, dc, mosi, sclk, rst);

String currentParameter = "";
String currentValue = "";
float currentFloatValue = 0.0;
String currentPgmNum = "";
String currentPatchName = "";
String newPatchName = "";
char * currentSettingsOption = "";
char * currentSettingsValue = "";
int currentSettingsPart = SETTINGS;
int paramType = PARAMETER;

boolean voiceOn[NO_OF_VOICES] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
boolean MIDIClkSignal = false;

unsigned long timer = 0;

void startTimer(){
  if (state == PARAMETER)
  {
    timer = millis();
  }
}

void renderBootUpPage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.drawRect(42, 30, 46, 11, ST7735_WHITE);
  tft.fillRect(88, 30, 61, 11, ST7735_WHITE);
  tft.setCursor(45, 31);
  tft.setFont(&Org_01);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.println("ELECTRO");
  tft.setTextColor(ST7735_BLACK);
  tft.setCursor(91, 37);
  tft.println("TECHNIQUE");
  tft.setTextColor(ST7735_YELLOW);
  tft.setFont(&Yeysk16pt7b);
  tft.setCursor(5, 70);
  tft.setTextSize(1);
  tft.println("TSynth");
  tft.setTextColor(ST7735_RED);
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(110, 95);
  tft.println(VERSION);
}

void renderCurrentPatchPage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSansBold18pt7b);
  tft.setCursor(5, 53);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.println(currentPgmNum);

  tft.setTextColor(ST7735_BLACK);
  tft.setFont(&Org_01);

  if (MIDIClkSignal) {
    tft.fillRect(93, 28, 19, 7, ST77XX_ORANGE);
    tft.setCursor(94, 33);
    tft.println("CLK");
  }

  tft.drawRect(115, 28, 12, 12, ST7735_BLUE);
  tft.drawRect(130, 28, 12, 12, ST7735_BLUE);
  tft.drawRect(145, 28, 12, 12, ST7735_BLUE);
  tft.drawRect(115, 43, 12, 12, ST7735_BLUE);
  tft.drawRect(130, 43, 12, 12, ST7735_BLUE);
  tft.drawRect(145, 43, 12, 12, ST7735_BLUE);


  if (voiceOn[0])
  {
    tft.fillRect(115, 28, 12, 12, ST7735_BLUE);
    tft.setCursor(120, 36);
    tft.println("1");
  }
  if (voiceOn[1])
  {
    tft.fillRect(130, 28, 12, 12, ST7735_BLUE);
    tft.setCursor(133, 36);
    tft.println("2");
  }
  if (voiceOn[2])
  {
    tft.fillRect(145, 28, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 36);
    tft.println("3");
  }
  if (voiceOn[3])
  {
    tft.fillRect(115, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(118, 51);
    tft.println("4");
  }
  if (voiceOn[4])
  {
    tft.fillRect(130, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(133, 51);
    tft.println("5");
  }
  if (voiceOn[5])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("6");
  }

  if (voiceOn[6])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("7");
  }

  if (voiceOn[7])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("8");
  }
  if (voiceOn[8])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("9");
  }
  if (voiceOn[9])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("10");
  }

  if (voiceOn[10])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("11");
  }
  if (voiceOn[11])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("12");
  }
  if (voiceOn[12])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("13");
  }
  if (voiceOn[13])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("14");
  }

  if (voiceOn[14])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("15");
  }
  if (voiceOn[15])
  {
    tft.fillRect(145, 43, 12, 12, ST7735_BLUE);
    tft.setCursor(148, 51);
    tft.println("16");
  }
  tft.drawFastHLine(10, 62, tft.width() - 20, ST7735_RED);
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(ST7735_YELLOW);
  tft.setCursor(1, 90);
  tft.setTextColor(ST7735_WHITE);
  tft.println(currentPatchName);
}

void renderPulseWidth(float value)
{
  tft.drawFastHLine(108, 74, 15 + (value * 13), ST7735_CYAN);
  tft.drawFastVLine(123 + (value * 13), 74, 20, ST7735_CYAN);
  tft.drawFastHLine(123 + (value * 13), 94, 16 - (value * 13), ST7735_CYAN);
  if (value < 0)
  {
    tft.drawFastVLine(108, 74, 21, ST7735_CYAN);
  }
  else
  {
    tft.drawFastVLine(138, 74, 21, ST7735_CYAN);
  }
}

void renderVarTriangle(float value)
{
  tft.drawLine(110, 94, 123 + (value * 13), 74, ST7735_CYAN);
  tft.drawLine(123 + (value * 13), 74, 136, 94, ST7735_CYAN);
}

void renderEnv(float att, float dec, float sus, float rel)
{
  tft.drawLine(100, 94, 100 + (att * 15), 74, ST7735_CYAN);
  tft.drawLine(100 + (att * 15), 74.0, 100 + ((att + dec) * 15), 94 - (sus * 20), ST7735_CYAN);
  tft.drawFastHLine(100 + ((att + dec) * 15), 94 - (sus * 20), 40 - ((att + dec) * 15), ST7735_CYAN);
  tft.drawLine(139, 94 - (sus * 20), 139 + (rel * 13), 94, ST7735_CYAN);
}

void renderCurrentParameterPage()
{
  switch (state)
  {
    case PARAMETER:
      tft.fillScreen(ST7735_BLACK);
      tft.setFont(&FreeSans12pt7b);
      tft.setCursor(0, 53);
      tft.setTextColor(ST7735_YELLOW);
      tft.setTextSize(1);
      tft.println(currentParameter);
      tft.drawFastHLine(10, 62, tft.width() - 20, ST7735_RED);
      tft.setCursor(1, 90);
      tft.setTextColor(ST7735_WHITE);
      tft.println(currentValue);
      switch (paramType)
      {
        case PULSE:
          renderPulseWidth(currentFloatValue);
          break;
        case VAR_TRI:
          renderVarTriangle(currentFloatValue);
          break;
        case FILTER_ENV:
          renderEnv(filterAttack * 0.0001, filterDecay * 0.0001, filterSustain, filterRelease * 0.0001);
          break;
        case AMP_ENV:
          renderEnv(ampAttack * 0.0001, ampDecay * 0.0001, ampSustain, ampRelease * 0.0001);
          break;
      }
      break;
  }
}

void renderDeletePatchPage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSansBold18pt7b);
  tft.setCursor(5, 53);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.println("Delete?");
  tft.drawFastHLine(10, 60, tft.width() - 20, ST7735_RED);
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(0, 78);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches.last().patchNo);
  tft.setCursor(35, 78);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches.last().patchName);
  tft.fillRect(0, 85, tft.width(), 23, ST77XX_DARKRED);
  tft.setCursor(0, 98);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches.first().patchNo);
  tft.setCursor(35, 98);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches.first().patchName);
}

void renderDeleteMessagePage() {
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSans12pt7b);
  tft.setCursor(2, 53);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.println("Renumbering");
  tft.setCursor(10, 90);
  tft.println("SD Card");
}

void renderSavePage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSansBold18pt7b);
  tft.setCursor(5, 53);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.println("Save?");
  tft.drawFastHLine(10, 60, tft.width() - 20, ST7735_RED);
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(0, 78);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches[patches.size() - 2].patchNo);
  tft.setCursor(35, 78);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches[patches.size() - 2].patchName);
  tft.fillRect(0, 85, tft.width(), 23, ST77XX_DARKRED);
  tft.setCursor(0, 98);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches.last().patchNo);
  tft.setCursor(35, 98);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches.last().patchName);
}

void renderReinitialisePage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(5, 53);
  tft.println("Initialise to");
  tft.setCursor(5, 90);
  tft.println("panel setting");
}

void renderPatchNamingPage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 53);
  tft.println("Rename Patch");
  tft.drawFastHLine(10, 62, tft.width() - 20, ST7735_RED);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5, 90);
  tft.println(newPatchName);
}

void renderRecallPage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSans9pt7b);
  tft.setCursor(0, 45);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches.last().patchNo);
  tft.setCursor(35, 45);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches.last().patchName);

  tft.fillRect(0, 56, tft.width(), 23, 0xA000);
  tft.setCursor(0, 72);
  tft.setTextColor(ST7735_YELLOW);
  tft.println(patches.first().patchNo);
  tft.setCursor(35, 72);
  tft.setTextColor(ST7735_WHITE);
  tft.println(patches.first().patchName);

  tft.setCursor(0, 98);
  tft.setTextColor(ST7735_YELLOW);
  patches.size() > 1 ? tft.println(patches[1].patchNo) : tft.println(patches.last().patchNo);
  tft.setCursor(35, 98);
  tft.setTextColor(ST7735_WHITE);
  patches.size() > 1 ? tft.println(patches[1].patchName) : tft.println(patches.last().patchName);
}

void showRenamingPage(String newName)
{
  newPatchName = newName;
}

void renderUpDown(uint16_t  x, uint16_t  y, uint16_t  colour)
{
  //Produces up/down indicator glyph at x,y
  tft.setCursor(x, y);
  tft.fillTriangle(x, y, x + 8, y - 8, x + 16, y, colour);
  tft.fillTriangle(x, y + 4, x + 8, y + 12, x + 16, y + 4, colour);
}


void renderSettingsPage()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setFont(&FreeSans12pt7b);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(0, 53);
  tft.println(currentSettingsOption);
  if (currentSettingsPart == SETTINGS) renderUpDown(140, 42, ST7735_YELLOW);
  tft.drawFastHLine(10, 62, tft.width() - 20, ST7735_RED);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5, 90);
  tft.println(currentSettingsValue);
  if (currentSettingsPart == SETTINGSVALUE) renderUpDown(140, 80, ST7735_WHITE);
}

void showCurrentParameterPage(const char *param, float val, int pType)
{
  currentParameter = param;
  currentValue = String(val);
  currentFloatValue = val;
  paramType = pType;
  startTimer();
}

void showCurrentParameterPage(const char *param, String val, int pType)
{
  if (state == SETTINGS || state == SETTINGSVALUE)state = PARAMETER;//Exit settings page if showing
  currentParameter = param;
  currentValue = val;
  paramType = pType;
  startTimer();
}

void showCurrentParameterPage(const char *param, String val)
{
  showCurrentParameterPage(param, val, PARAMETER);
}

void showPatchPage(String number, String patchName)
{
  currentPgmNum = number;
  currentPatchName = patchName;
}

void showSettingsPage(char *  option, char * value, int settingsPart)
{
  currentSettingsOption = option;
  currentSettingsValue = value;
  currentSettingsPart = settingsPart;
}

void displayThread()
{
  threads.delay(2000); //Give bootup page chance to display
  while (1)
  {
    switch (state)
    {
      case PARAMETER:
        if ((millis() - timer) > DISPLAYTIMEOUT)
        {
          renderCurrentPatchPage();
        }
        else
        {
          renderCurrentParameterPage();
        }
        break;
      case RECALL:
        renderRecallPage();
        break;
      case SAVE:
        renderSavePage();
        break;
      case REINITIALISE:
        renderReinitialisePage();
        tft.updateScreen(); //update before delay
        threads.delay(1000);
        state = PARAMETER;
        break;
      case PATCHNAMING:
        renderPatchNamingPage();
        break;
      case PATCH:
        renderCurrentPatchPage();
        break;
      case DELETE:
        renderDeletePatchPage();
        break;
      case DELETEMSG:
        renderDeleteMessagePage();
        break;
      case SETTINGS:
      case SETTINGSVALUE:
        renderSettingsPage();
        break;
    }
    tft.updateScreen();
  }
}

void setupDisplay() {
  tft.useFrameBuffer(true);
  tft.initR(INITR_GREENTAB);
  tft.setRotation(3);
  tft.invertDisplay(true);
  renderBootUpPage();
  tft.updateScreen();
  threads.addThread(displayThread);
}