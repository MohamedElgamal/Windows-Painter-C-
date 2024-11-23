//Mohamed Abdullah Saad Mohamed Ismail Elgamal
// 16101277
// Class B
//Computer Graphics Project

#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include<GL/glut.h>
#include <stdio.h>
#include <string.h>
#include<math.h>
#define ROW_RECT_POINTS 8
#define COL_RECT_POINTS 4
#define ROW_PRIMARY_COLOR 8
#define COL_PRIMARY_COLOR 3
#define MAX_SHAPES 1000
/**
    **************************Important this line is responsible of pointing to function which will be responsible to draw the shape******************************
**/
typedef void (*FunctionCall)(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str);

int phyWidth= 700;
int phyHeight= 700;
int logWidth=100;
int logHeight=100;

int mouseX , mouseY;
int centerX = logWidth/2;
int centerY = logHeight/2;
int mouseHasClicked = 0 , mouseHasReleased = 0;
int mouseXStart , mouseYStart , mouseXEnd , mouseYEnd;
int startX = 1 , endX = 99  , startY = 1  , endY = 85 ;
int textButtonHasSelected = 0;//
char keyStrick[1000];
char keyPressed;//
int sizeOfKeyStrick = 0;
int indexOfSelectedButton = -1;
int selectedFillButton = -1;
int fillStateTriggerButtonColor = 0;
const int guiRectPointsLocation[ROW_RECT_POINTS][COL_RECT_POINTS] = {
                                                                { 1 , 11 , 94 , 99 },
                                                                { 12 , 22 , 94 , 99 },
                                                                { 23 , 33 , 94 , 99 },
                                                                { 34 , 44 , 94 , 99 },
                                                                { 1 , 11 , 88 , 93 },
                                                                { 12 , 22 , 88 , 93 },
                                                                { 23 , 33 , 88 , 93 },
                                                                { 34 , 44 , 88 , 93 }
                                                                    };
const int guiColorRectPointsLocation[ROW_RECT_POINTS][ROW_RECT_POINTS] = {
                                                                { 50 , 53 , 92 , 95 },
                                                                { 54 , 57 , 92 , 95 },
                                                                { 58 , 61 , 92 , 95 },
                                                                { 62 , 65, 92 , 95 },
                                                                { 66 , 69 , 92 , 95 },
                                                                { 70 , 73 , 92 , 95 },
                                                                { 74 , 77 , 92 , 95 },
                                                                { 78 , 81 , 92 , 95 }
                                                                    };
const float primaryColors[ROW_PRIMARY_COLOR][COL_PRIMARY_COLOR]= {
                                                                { 1.0 , 0.0 , 0.0 },
                                                                { 0.57 , 0.81 , 0.31 },
                                                                { 0.0 , 0.69 , 0.94 },
                                                                { 1.0 , 1.0 , 0.0 },
                                                                { 0.89 , 0.41 , 0.03 },
                                                                { 0.43 , 0.18 , 0.62 },
                                                                { 1.0 , 1.0 , 1.0 },
                                                                { 0.0 , 0.0 ,0.0 }
                                                                    };

const int guiTextRectLocation[ROW_RECT_POINTS][2] = {
                                                                { 2, 96 },
                                                                { 13 , 96 },
                                                                { 24 , 96 },
                                                                { 35 , 96 },
                                                                { 2 , 90 },
                                                                { 13 , 90 },
                                                                { 24 , 90 },
                                                                { 35 , 90 }
                                                                    };
char *guiTextRect[ROW_RECT_POINTS] = {"Line" , "Rect" , "Circle" , "Fill" , "Pen" , "Text" , "Undo" , "No Fill" };


typedef struct ClickedButtonProperty
{
    /** SELECTED RGB COLOR DEFAULT SHOULD BE BLACK**/
    float colorRed ;
    float colorGreen ;
    float colorBlue ;
    /** Shape type which determine which shape will be draw rect , circle, etc...**/
    int functionSelected;
    int colorSelected;
    int fillState;// -1 no fill property has selected , 0 no fill , 1 fill
    bool buttonHasClicked;
    /**  Indicate if the current clicked button is one of the drawing functions shapes (circle , rect , etc ..) **/
    int drawingState;
    /** cache function call of the function that will be executed**/
    FunctionCall functionCall;
}ClickedButtonProperty;
ClickedButtonProperty clickedButtonProperty;

typedef struct Shapes
{
    float r;
    float g;
    float b;
    FunctionCall functionCall; //call to functions which will draw the shape
    int shapePoints[1000][2];
    int currrentPointsCount;
    char *str;
}Shapes;
Shapes shapes[MAX_SHAPES];
int currentShapesLength = 0;

void setDefaultValues(); /**Responsible of Setting the color value to black color at start**/
void setFunctionCall(int i);
void setShapeValue(int xPoint , int yPoint , int xEndPoint , int yEndPoint);
// functions which is responsible of drawing GUI
void drawGuiRect(int x1 , int x2 , int y1 , int y2);
void drawGuiColorRect(int x1 , int x2 , int y1 , int y2 , float r , float g , float b);
void drawDrawingAreaFrame();
void drawAllGui();

//Main Functionality functions
bool shapeTypeButtonClicked(int xPoint , int yPoint);
bool colorButtonClicked(int xPoint , int yPoint);
bool drawingAreaClicked(int xPoint , int yPoint);
void hoverOverButton(int xPoint , int yPoint);
void changerColorOfButtonPressed();
void redrawBufferedShapes();
void cleanKeyStrickArray();
void shapesOutsideDrawingArea(); /**Hand if there is a part of the shape outside drawing area to remove this part**/

/**Functions which is responsible of drawing shapes**/
void drawLine(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str);
void drawNotFillRect(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str);
void drawFillRect(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str);
void drawNotFillCircle(int xPoint , int yPoint , int xEndPoint , int yEndPoint, int index , char *str);
void drawFillCircle(int xPoint , int yPoint , int xEndPoint , int yEndPoint, int index , char *str);
void drawUsingPen(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str);
void notFillState(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str);
void drawText(int xPoint , int yPoint,  int xEndPoint , int yEndPoint ,int index , char *str);
/**This Function is responsible of disallow program to crash if no button is no
    drawing button has selected and user try to click on the drawing area of screen**/
void noAction(int xPoint , int yPoint,  int xEndPoint , int yEndPoint ,int index , char *str);
void undoShape();

// Main Program Functions

void init()
{
	glClearColor(1.0 , 1.0 , 1.0 , 1.0);
	glMatrixMode( GL_PROJECTION);
	gluOrtho2D( 0.0, logWidth, 0.0, logHeight);
}

static void display(void)
{
        glClear( GL_COLOR_BUFFER_BIT);
        redrawBufferedShapes();
        shapesOutsideDrawingArea();
        hoverOverButton(mouseXEnd ,mouseYEnd);
        drawAllGui();
        changerColorOfButtonPressed();
         if(mouseHasClicked == 1){
                setShapeValue(mouseXStart , mouseYStart , mouseXEnd , mouseYEnd);
                clickedButtonProperty.functionCall(mouseXStart , mouseYStart , mouseXEnd , mouseYEnd , currentShapesLength , keyStrick);
        }
        glutSwapBuffers();
}

void invertMouseCoordinates(int x , int y)
{
    mouseX = x;
    mouseX = 0.5 + 1.0 * mouseX * logWidth / phyWidth;
    mouseY = phyHeight- y;
    mouseY = 0.5 + 1.0 * mouseY * logHeight / phyHeight;
}

void mouseClick(int btn, int state, int x, int y)
{
    invertMouseCoordinates(x , y);
    if(state==GLUT_DOWN && btn==GLUT_LEFT_BUTTON )
    {
        if( drawingAreaClicked(mouseX , mouseY) )
        {
           if(mouseHasReleased == 0)
            {
                mouseXStart = mouseX;
                mouseYStart = mouseY;
            }
            mouseHasClicked = 1;
            mouseHasReleased++;
        }
        colorButtonClicked(mouseX , mouseY);
        shapeTypeButtonClicked(mouseX , mouseY);
    }
    else if(state==GLUT_UP && btn==GLUT_LEFT_BUTTON && drawingAreaClicked(mouseX , mouseY))
    {
        mouseHasReleased = 0;
        mouseHasClicked = 0;
        cleanKeyStrickArray();
        /** Increase the current index of number of shapes that has been drawn**/
        currentShapesLength++;
    }

     glutPostRedisplay();
}

void mouseMovement(int x , int y)
{
    mouseXEnd =  ( x * logWidth ) / phyWidth;
    y = phyHeight - y;
    mouseYEnd = ( y * logHeight ) / phyHeight;
    glutPostRedisplay();
}

void passiveMoving(int x , int y)
{
    mouseXEnd =  ( x * logWidth ) / phyWidth;
    y = phyHeight - y;
    mouseYEnd = ( y * logHeight ) / phyHeight;
    glutPostRedisplay();
}

void keyBoard(unsigned char key , int x , int y)
{
    keyStrick[sizeOfKeyStrick++] = key;
    glutPostRedisplay();
}

void drawGuiText(int x , int y , char *str)
{
    int length = strlen(str);
    glColor3f (0.0 , 0.0 , 0.0);
    glRasterPos2d(x,y);
    for (int i=0; i< length ;i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[i]);
    glFlush();
}

int main(int argc, char *argv[])
{
    setDefaultValues();
    glutInit( &argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE| GLUT_RGB);
	glutInitWindowPosition( 100, 100);
	glutInitWindowSize( phyWidth, phyHeight);
	glutCreateWindow( "Your Painter");
	glutDisplayFunc(display);
	init();
    glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMovement);//glutMotionFunc(passiveMoving);
	glutPassiveMotionFunc(passiveMoving);
	glutKeyboardFunc(keyBoard);
	glutMainLoop();
}

void setDefaultValues()
{
    clickedButtonProperty.colorRed = 0.0f;
    clickedButtonProperty.colorGreen = 0.0f;
    clickedButtonProperty.colorBlue = 0.0f;
    clickedButtonProperty.colorSelected = 7;
    clickedButtonProperty.functionCall = noAction;
    clickedButtonProperty.fillState = -1;
}

/************Implementation Functions Area ****************/

void drawGuiRect(int x1 , int x2 , int y1 , int y2)
{
    glColor3f(0.0 , 0.0 , 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x1 , y1);
    glVertex2i(x1 , y2);
    glVertex2i(x2 , y2);
    glVertex2i(x2 , y1);
    glEnd();
}

void drawGuiColorRect(int x1 , int x2 , int y1 , int y2 , float r , float g , float b)
{
    glColor3f(r , g , b);
    glBegin(GL_POLYGON);
    glVertex2i(x1 , y1);
    glVertex2i(x1 , y2);
    glVertex2i(x2 , y2);
    glVertex2i(x2 , y1);
    glEnd();
}

void drawDrawingAreaFrame(){
    glColor3f(0.0 , 0.0 , 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(startX , startY);
    glVertex2i(startX , endY);
    glVertex2i(endX , endY );
    glVertex2i(endX , startY);
    glEnd();
}

void drawAllGui()
{
    for(int i = 0 ; i < ROW_RECT_POINTS ; i++)//drawing the 8 GUI Rectangles in there correct locations
    {
        drawGuiRect(guiRectPointsLocation[i][0] ,guiRectPointsLocation[i][1]
                    ,guiRectPointsLocation[i][2] , guiRectPointsLocation[i][3]);
        char *temp = guiTextRect[i];
        drawGuiText(guiTextRectLocation[i][0] ,guiTextRectLocation[i][1] , temp);
    }
    for(int i = 0 ; i < ROW_RECT_POINTS ; i++)
    {
        drawGuiColorRect(guiColorRectPointsLocation[i][0] , guiColorRectPointsLocation[i][1],
                        guiColorRectPointsLocation[i][2] , guiColorRectPointsLocation[i][3],
                        primaryColors[i][0] , primaryColors[i][1] , primaryColors[i][2]);
        drawGuiRect(guiColorRectPointsLocation[i][0] , guiColorRectPointsLocation[i][1],
                        guiColorRectPointsLocation[i][2] , guiColorRectPointsLocation[i][3]);
    }

    drawDrawingAreaFrame();
}

/*******************Functionality Functions***********************/

bool drawingAreaClicked(int xPoint , int yPoint)
{
    if( startX <= xPoint&& endX >= xPoint
           &&startY <= yPoint && endY >= yPoint )
        { /**Points inside the area that available for drawing**/
            return true;
        }
    return false;
}

bool shapeTypeButtonClicked(int xPoint , int yPoint)
{
    for(int i = 0 ; i < ROW_RECT_POINTS ; i++)
    {

        if(guiRectPointsLocation[i][0] <= xPoint&& guiRectPointsLocation[i][1] >= xPoint
           &&guiRectPointsLocation[i][2] <= yPoint && guiRectPointsLocation[i][3] >= yPoint && i == 6 ) /** Undo Button Has Clicked **/
        {
            undoShape();
        }
        else if( guiRectPointsLocation[i][0] <= xPoint&& guiRectPointsLocation[i][1] >= xPoint
           &&guiRectPointsLocation[i][2] <= yPoint && guiRectPointsLocation[i][3] >= yPoint )
        { /**Point of mouse inside the rect may be hovered or clicked**/
            clickedButtonProperty.functionSelected = i;
            clickedButtonProperty.buttonHasClicked = true;
            setFunctionCall(i);
            indexOfSelectedButton = i;
            return true;
        }
    }
    return false;
}

bool colorButtonClicked(int xPoint , int yPoint)
{
    for(int i = 0 ; i < ROW_PRIMARY_COLOR ; i++)
    {
        if( guiColorRectPointsLocation[i][0] <= xPoint&& guiColorRectPointsLocation[i][1] >= xPoint
           &&guiColorRectPointsLocation[i][2] <= yPoint && guiColorRectPointsLocation[i][3] >= yPoint )
        { /**Point of mouse inside the rect may be hovered or clicked**/
            clickedButtonProperty.colorSelected = i;
            return true;
        }
    }
    return false;
}

void setFunctionCall(int i )
{
    switch(i)
    {
        case 0 : clickedButtonProperty.functionCall = drawLine;
                    clickedButtonProperty.fillState = -1;
                    fillStateTriggerButtonColor = 0;
                    break;
        case 1 :if(fillStateTriggerButtonColor == 0)
                {
                    clickedButtonProperty.fillState = 0;
                    fillStateTriggerButtonColor = 1;
                }
                if(clickedButtonProperty.fillState == 0) clickedButtonProperty.functionCall = drawNotFillRect;
                else clickedButtonProperty.functionCall = drawFillRect;
                    break;
        case 2 :
                if(fillStateTriggerButtonColor == 0)
                {
                    clickedButtonProperty.fillState = 0;
                    fillStateTriggerButtonColor = 1;
                }
                if(clickedButtonProperty.fillState == 0)
                    clickedButtonProperty.functionCall = drawNotFillCircle;
                else
                    clickedButtonProperty.functionCall = drawFillCircle;
                break;
        case 3 : clickedButtonProperty.fillState = 1;
                if(clickedButtonProperty.functionCall == drawNotFillRect)
                        clickedButtonProperty.functionCall = drawFillRect;
                else if(clickedButtonProperty.functionCall == drawNotFillCircle)
                        clickedButtonProperty.functionCall = drawFillCircle;
                fillStateTriggerButtonColor = 1;
                break;
        case 4 : clickedButtonProperty.functionCall = drawUsingPen;
                clickedButtonProperty.fillState = -1;
                fillStateTriggerButtonColor = 0;
                break;
        case 5 : clickedButtonProperty.functionCall = drawText;
                clickedButtonProperty.fillState = -1;
                fillStateTriggerButtonColor = 0;
                break;
        case 6 : clickedButtonProperty.fillState = -1;
                fillStateTriggerButtonColor = 0;
                break;
        case 7 : clickedButtonProperty.fillState = 0;
                 if(clickedButtonProperty.functionCall == drawFillRect)
                        clickedButtonProperty.functionCall = drawNotFillRect;
                else if(clickedButtonProperty.functionCall == drawFillCircle)
                        clickedButtonProperty.functionCall = drawNotFillCircle;
                 break;
        default : break;
    }
}

void setShapeValue( int xPoint , int yPoint , int xEndPoint , int yEndPoint){
    shapes[currentShapesLength].r = primaryColors[clickedButtonProperty.colorSelected][0];
    shapes[currentShapesLength].g  = primaryColors[clickedButtonProperty.colorSelected][1];
    shapes[currentShapesLength].b  = primaryColors[clickedButtonProperty.colorSelected][2];
    shapes[currentShapesLength].functionCall = clickedButtonProperty.functionCall;
    if(shapes[currentShapesLength].functionCall == drawUsingPen )
    {
        int counter = shapes[currentShapesLength].currrentPointsCount;
        shapes[currentShapesLength].shapePoints[counter][0] = xEndPoint;
        shapes[currentShapesLength].shapePoints[counter][1] = yEndPoint;
        shapes[currentShapesLength].currrentPointsCount +=1;
        shapes[currentShapesLength].str = "";
    }
    else
    {
        shapes[currentShapesLength].shapePoints[0][0] = xPoint;
        shapes[currentShapesLength].shapePoints[0][1] = yPoint;
        shapes[currentShapesLength].shapePoints[1][0] = xEndPoint;
        shapes[currentShapesLength].shapePoints[1][1] = yEndPoint;
        shapes[currentShapesLength].str = "";
    }
    if(shapes[currentShapesLength].functionCall == drawText)
    {
        shapes[currentShapesLength].str = keyStrick;
    }
}

void drawLine(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str){
    glColor3f(shapes[index].r , shapes[index].g , shapes[index].b);
    glBegin(GL_LINES);
    glVertex2i(xPoint , yPoint);
    glVertex2i( xEndPoint , yEndPoint);
    glEnd();
}

void drawNotFillRect(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str){
    glColor3f(shapes[index].r , shapes[index].g , shapes[index].b);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xPoint , yPoint);
    glVertex2i(xPoint , yEndPoint);
    glVertex2i(xEndPoint , yEndPoint);
    glVertex2i(xEndPoint , yPoint);
    glEnd();
}

void drawNotFillCircle(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str){

    int num_segments = 50 ;
    int xDiff = xEndPoint - xPoint , yDiff = yEndPoint - yPoint;
    glColor3f(shapes[index].r , shapes[index].g , shapes[index].b);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = xDiff * cosf(theta);
        float y = yDiff * sinf(theta);
        glVertex2f(x + xPoint, y + yPoint);
    }
    glEnd();
}

void drawFillRect(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str){
    glColor3f(shapes[index].r , shapes[index].g , shapes[index].b);
    glBegin(GL_POLYGON);
    glVertex2i(xPoint , yPoint);
    glVertex2i(xPoint , yEndPoint);
    glVertex2i(xEndPoint , yEndPoint);
    glVertex2i(xEndPoint , yPoint);
    glEnd();
}

void drawFillCircle(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str){
    int num_segments = 50 ;
    int xDiff = xEndPoint - xPoint , yDiff = yEndPoint - yPoint;
    glColor3f(shapes[index].r , shapes[index].g , shapes[index].b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++)
        {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = xDiff * cosf(theta);
        float y = yDiff * sinf(theta);
        glVertex2f(x + xPoint, y + yPoint);
    }
    glEnd();
}

void drawUsingPen(int xPoint , int yPoint , int xEndPoint , int yEndPoint , int index , char *str){
    glColor3f(shapes[index].r , shapes[index].g , shapes[index].b);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    for(int i = 0 ; i < shapes[index].currrentPointsCount ; i++)
        glVertex2i(shapes[index].shapePoints[i][0] , shapes[index].shapePoints[i][1]);
    glEnd();
}

void drawText(int xPoint , int yPoint,  int xEndPoint , int yEndPoint, int index , char *str){
    int length =  strlen(str) ;
    glColor3f(shapes[index].r , shapes[index].g , shapes[index].b);
    glRasterPos2d(xPoint,yPoint);
    for (int i = 0 ; i < length; i++)
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12 , str[i] );
}
void noAction(int xPoint , int yPoint,  int xEndPoint , int yEndPoint ,int index , char *str){}
void undoShape(){
    if(currentShapesLength != 0)
    {
        currentShapesLength--;
        if(shapes[currentShapesLength].functionCall == drawUsingPen)
        {
            for(int i = 0 ; i < shapes[currentShapesLength].currrentPointsCount ; i++ )
            {
                shapes[currentShapesLength].shapePoints[i][0] = 0;
                shapes[currentShapesLength].shapePoints[i][1] = 0;
            }
            shapes[currentShapesLength].currrentPointsCount = 0;
        }
    }
}

void redrawBufferedShapes(){
    for(int i = 0 ; i < currentShapesLength ; i++ )
    {
        if(shapes[i].functionCall == drawUsingPen)
        {
            for(int j = 0 ; j < shapes[i].currrentPointsCount ; j++)
                {
                    shapes[i].functionCall(0 ,0, shapes[i].shapePoints[j][0] , shapes[i].shapePoints[j][1], i ,shapes[i].str);
                }
        }
        else
        {
            shapes[i].functionCall(shapes[i].shapePoints[0][0] , shapes[i].shapePoints[0][1]
                                , shapes[i].shapePoints[1][0] , shapes[i].shapePoints[1][1], i ,shapes[i].str);
        }
    }
}

void hoverOverButton(int xPoint , int yPoint)
{
    for(int i = 0 ; i < ROW_RECT_POINTS ; i++)
    {
        if( guiRectPointsLocation[i][0] <= xPoint&& guiRectPointsLocation[i][1] >= xPoint
           &&guiRectPointsLocation[i][2] <= yPoint && guiRectPointsLocation[i][3] >= yPoint )
        {
            glColor3f(0.4 , 0.4 , 0.4);
            glBegin(GL_POLYGON);
            glVertex2i(guiRectPointsLocation[i][0] , guiRectPointsLocation[i][2] );
            glVertex2i(guiRectPointsLocation[i][0]  , guiRectPointsLocation[i][3] );
            glVertex2i(guiRectPointsLocation[i][1]  , guiRectPointsLocation[i][3] );
            glVertex2i(guiRectPointsLocation[i][1]  , guiRectPointsLocation[i][2] );
            glEnd();
        }
    }
}
void shapesOutsideDrawingArea()
{
    glColor3f(1 , 1 , 1);
    glBegin(GL_POLYGON);
    glVertex2i(0 , 85);
    glVertex2i(0 ,100);
    glVertex2i(100 , 100);
    glVertex2i(100 , 85);
    glEnd();
}
void changerColorOfButtonPressed()
{
    int i = indexOfSelectedButton;

    if( i != 6)
    {
        glColor3f(0.6 , 0.4 , 1);
        glBegin(GL_LINE_LOOP);
        glVertex2i(guiRectPointsLocation[i][0] , guiRectPointsLocation[i][2] );
        glVertex2i(guiRectPointsLocation[i][0]  , guiRectPointsLocation[i][3] );
        glVertex2i(guiRectPointsLocation[i][1]  , guiRectPointsLocation[i][3] );
        glVertex2i(guiRectPointsLocation[i][1]  , guiRectPointsLocation[i][2] );
        glEnd();
    }
    if(clickedButtonProperty.fillState == 0)
    {
        glColor3f(0.6 , 0.4 , 1);
        glBegin(GL_LINE_LOOP);
        glVertex2i(guiRectPointsLocation[7][0] , guiRectPointsLocation[7][2] );
        glVertex2i(guiRectPointsLocation[7][0]  , guiRectPointsLocation[7][3] );
        glVertex2i(guiRectPointsLocation[7][1]  , guiRectPointsLocation[7][3] );
        glVertex2i(guiRectPointsLocation[7][1]  , guiRectPointsLocation[7][2] );
        glEnd();
    }
    else if(clickedButtonProperty.fillState == 1)
    {
        glColor3f(0.6 , 0.4 , 1);
        glBegin(GL_LINE_LOOP);
        glVertex2i(guiRectPointsLocation[3][0] , guiRectPointsLocation[3][2] );
        glVertex2i(guiRectPointsLocation[3][0]  , guiRectPointsLocation[3][3] );
        glVertex2i(guiRectPointsLocation[3][1]  , guiRectPointsLocation[3][3] );
        glVertex2i(guiRectPointsLocation[3][1]  , guiRectPointsLocation[3][2] );
        glEnd();
    }
}

void cleanKeyStrickArray()
{
    for(int i = 0 ; i < sizeOfKeyStrick ; i++)
    {
        keyStrick[i] = NULL;
    }
    sizeOfKeyStrick = 0;
}
