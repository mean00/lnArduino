#include "simplerST7735.h"

/**
 * \fn checkFont
 * \brief extract max width/ max height from the font
 */
static void checkFont(const GFXfont *font, st7735::FontInfo *info)
{
    int mW=0,mH=0;
    int x,y;
   for(int i=font->first;i<font->last;i++)
   {
         GFXglyph *glyph  = font->glyph+i-font->first;
         x=glyph->xAdvance;
         y=-glyph->yOffset;
         if(x>mW) mW=x;         
         if(y>mH) mH=y;
   }
    info->maxHeight=mH + 1;
    info->maxWidth=mW;    
    info->font=font;
}

void  st7735::print(int x, int y,const char *z)
{
    cursor_x=x;
    cursor_y=y;
   int l=strlen(z);
   csOn();
   while(*z)
   {
       int inc=writeChar(*z);
       cursor_x+=inc;
       z++;
   }
   csOff();
}

/**
 * 
 * @param small
 * @param medium
 * @param big
 */
void  st7735::setFontFamily(const GFXfont *small, const GFXfont *medium, const GFXfont *big)
{
    checkFont(small, fontInfo+0);
    checkFont(medium,fontInfo+1);
    checkFont(big,   fontInfo+2);
}       

/**
 * \fn mySquare
 * \brief Draw a square of w*xheight size at position x,y
 * \param filler is a prefill color array
 */
int st7735::mySquare(int x, int y, int w, int xheight, uint16_t filler)
{
    if(w+x>=_width)
    {
        w=_width-x;
        if(w<=0) return 0;
    }
    if(xheight+y>=_height)
    {
        xheight=_height-y;
        if(xheight<=0)
            return 0;
    }
    setAddress(x,y,w, xheight);
    floodWords(filler,w*xheight);
    return 0;
}

#define cmdMode()  digitalWrite(_pinDC,LOW)
#define dataMode() digitalWrite(_pinDC,HIGH)
void     st7735::push2Colors(uint8_t *data, int len, boolean first,uint16_t fg, uint16_t bg)
{
    dataMode();
    for(int i=0;i<len;i++)
    {
        if(data[i])
            sendWord(fg);
        else
            sendWord(bg);
    }
}

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
 * 
 * @param size
 */
void  st7735::setFontSize(FontSize size)
{
    switch(size)
    {
        case SmallFont :  currentFont=fontInfo+0;break;
        default:
        case MediumFont :   currentFont=fontInfo+1;break;
        case BigFont :   currentFont=fontInfo+2;break;
    }    
    gfxFont=currentFont->font;
}
 
/**
 * 
 * @param c
 * @return 
 */
int st7735::writeChar(char c) 
{

    if (c == '\n') 
    {
      cursor_x = 0;
      cursor_y +=           gfxFont->yAdvance;
      return 1;
    } 
    if(c=='\r')
      return 1;
    uint8_t first = gfxFont->first;
    if ((c < first) || (c > gfxFont->last)) 
        return 1;
    
    GFXglyph *glyph = gfxFont->glyph + c-first;
    int w = glyph->width,   h = glyph->height;
    if ((w <= 0) || (h <= 0)) 
    {
        cursor_x += glyph->xAdvance ;    
        return 1;
    }

    int xo = glyph->xOffset; // sic
    if ( ((cursor_x +  (xo + w)) > 128)) 
    {
      cursor_x = 0;
      cursor_y +=   gfxFont->yAdvance;
    }
    //int st7735::myDrawChar(int x, int y, unsigned char c,  int color, int bg,FontInfo &infos)
    myDrawChar(cursor_x, cursor_y, c, _fg,_bg,*currentFont); 
    cursor_x += glyph->xAdvance ;    
    return 1;
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

    bool first=true;
    int nbPixel=widthInPixel*height;    
    int mask=0;
    int cur;       
    int repeat;
    bool color;
    for( int yy=0;yy<height;yy++)    
    {
        int column=64-(wy+yy);        
        int bitToChange=1<<(column%8);
        column=128*(column/8);
        int pack=wx;
        for(int xx=0;xx<widthInPixel;)
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
                        color=true;
                    }else
                        color=false;
                    mask>>=1;                
                    pack++;
                    xx++;                   
                    if(color) 
                    {
                        scrbuf[column+pack] |=bitToChange;                        
                    }
                    else 
                    {
                        scrbuf[column+pack] &=~bitToChange;                        
                    }
                    
                }
                    
            }
            
        }
    }
}
// EOF