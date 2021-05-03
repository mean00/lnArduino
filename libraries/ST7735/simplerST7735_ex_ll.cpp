#include "simplerST7735.h"
/**
 * 
 * @param x
 * @param y
 * @param c
 * @param color
 * @param bg
 * @param infos
 * @return 
 */
#define debug(...) {}
int st7735::myDrawChar(int x, int y, unsigned char c,  int color, int bg,FontInfo &infos)
{
    c -= infos.font->first;
    GFXglyph *glyph  = infos.font->glyph+c;
    
    uint8_t *p= infos.font->bitmap+glyph->bitmapOffset;        
    int  w  = glyph->width;
    int  h  = glyph->height;    
    uint8_t  column[_width];
    
    debug(uint16_t oldbg=bg);
    debug(bg=YELLOW);
    debug(color=GREEN);
    
    // Special case, space, it has yOffsset > 0
    if(infos.font->first+c==' ')
    {
        int adv=glyph->xAdvance;
        int top=infos.maxHeight;
         mySquare(x,y-top,
                  adv, //Fix!
                  top+2,bg);
         return adv;
    }
    
    // top & bottom
    int top=infos.maxHeight+glyph->yOffset;
    mySquare(x,y-infos.maxHeight,glyph->xAdvance,top,bg);

    int bottom=-glyph->yOffset-h;
    if(bottom>=-2)
        mySquare(x,y-bottom,glyph->xAdvance,bottom+2,bg);      
    

    y+= glyph->yOffset;   // offset is <0 most of the time
    
    int left=glyph->xOffset;
    int right=glyph->xAdvance-(w+left);
    if(right<0) right=0;
       
    int    finalColor;    
    int  bits = 0, bit = 0;
    setAddress(x,y, glyph->xAdvance, h);
    debug(bg=oldbg);
    uint8_t  *col=column;
    
    // Pre-fill & left /right
    memset(column,0,left);
    memset(column+left+w,0,right);
    // fill in body
    bool first=true;
    for( int line=h-1;line>=0;line--)
    {      
        col=column+left;     
        // mid
        for( int xcol=w-1;xcol>=0;xcol--)
        {
            if(!bit) // reload ?
            {
                bits= *p++;
                if(xcol>=8) // speed up some special cases
                {
                    switch(bits)
                    {
                     
                         case 0x0f:  
                                    *col++=0; *col++=0;*col++=0; *col++=0;
                                    *col++=1; *col++=1;*col++=1; *col++=1;
                                    xcol-=7;
                                    bit=0;
                                    continue;
                                    break;
                        case 0xf0:  
                                    *col++=1; *col++=1;*col++=1; *col++=1;
                                    *col++=0; *col++=0;*col++=0; *col++=0;
                                    xcol-=7;
                                    bit=0;
                                    continue;
                                    break;                                    
                        case 0xff:  
                                    *col++=1; *col++=1;*col++=1; *col++=1;
                                    *col++=1; *col++=1;*col++=1; *col++=1;
                                    xcol-=7;
                                    bit=0;
                                    continue;
                                    break;
                        case 0x00: 
                                    *col++=0;*col++=0;*col++=0;*col++=0;
                                    *col++=0;*col++=0;*col++=0;*col++=0;
                                    xcol-=7;
                                    bit=0;
                                    continue;
                                    break;
                        
                        default:break;
                    }
                }
                bit = 0x80;
            }      
                            
            *col++=!!(bits & bit) ;  
            bit=bit>>1;
        }
        // 9ms here
        push2Colors(column,glyph->xAdvance,first,color,bg);
        first=false;
    }   
    return glyph->xAdvance;
}


/**
 * This could be optimized ** A LOT ** by swapping x & y in the source image
 * @param widthInPixel
 * @param height
 * @param wx
 * @param wy
 * @param fgcolor
 * @param bgcolor
 * @param data
 */

void st7735::drawRLEBitmap(int widthInPixel, int height, int wx, int wy, int fgcolor, int bgcolor, const uint8_t *data)
{    
    
    int nbPixel=widthInPixel*height;
    int pixel=0;
    setAddress(wx, wy,  widthInPixel, height);
    dataMode();
    int mask=0;
    int cur;   
    uint16_t *o=scrbuf;
    int ready=0;
    int repeat;
    uint16_t color;
    while(pixel<nbPixel)        
    {
        // load next
        cur=*data++;
        if(cur==0x76)
        {
            cur=*data++;
            repeat=*data++;
        }else
        {
            repeat=1;
        }
        // 8 pixels at a time
        for(int r=0;r<repeat;r++)
        {
            int mask=0x80;
            for(int i=0;i<8;i++)
            {
                if(mask & cur)
                {
                    color=fgcolor;
                }else
                    color=0xff00*0+1*bgcolor;
                mask>>=1;
                *o++=color;
                ready++;
            }
            if(ready>(ST7735_BUFFER_SIZE_WORD-16))
            { // Flush
              sendWords(ready,scrbuf);
              ready=0;
              o=scrbuf;
            }
        }
        pixel+=repeat*8;
    }
    if(ready)
        sendWords(ready,scrbuf);
}


// EOF
