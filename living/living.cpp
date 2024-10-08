#include "living.h"
#include "stdio.h"

void Living :: make(){
    texture_source.w = width;
    texture_source.h = height;

    hit.w = width;
    hit.h = height;
}


void Living :: update(Table table){
    move(table);

    hit.x = x;
    hit.y = y;
    texture_source.y = animation * height;
    texture_source.x = sub_animation * width;

    Specific_update(table);
}

void Living :: render(SDL_Renderer *renderer, SDL_Texture *sheet, int off_x, int off_y){
    SDL_Rect temp = hit;
    temp.x += off_x;
    temp.y += off_y;

    SDL_RenderCopy(renderer, sheet, &texture_source, &temp);
}


void Living :: move(){
    x += speed_x;
    y += speed_y;
}

void Living :: move(Table table){
    double xTemp = speed_x;
    double yTemp = speed_y;
    double distTemp;

    if(speed_x > 0){
        distTemp = distanceRight(table);
        if(distTemp < xTemp)
            xTemp = distTemp;
    }
    else if(speed_x < 0){
        distTemp = distanceLeft(table);
        if(distTemp > xTemp)
            xTemp = distTemp;
    }
    if(speed_y > 0){
        distTemp = distanceDown(table);
        if(distTemp < yTemp)
            yTemp = distTemp;
    }
    else if(speed_y < 0){
        distTemp = distanceUp(table);
        if(distTemp > yTemp)
            yTemp = distTemp;
    }

    if(xTemp != 0 && yTemp != 0){
        int i = hit.x / Block().width;
        int j = hit.y / Block().height;
        if(xTemp > 0){
            i += hit.w / Block().width;
        }
        if(yTemp > 0){
            j += hit.h / Block().height;
        }
        if(*(table.blocks + i + (j * table.width)) != 0){
            if(xTemp > 0){
                if((int)xTemp > (i * Block().width) - hit.x - hit.w)
                    xTemp = (i * Block().width) - hit.x - hit.w;
            }
            else{
                if(xTemp < (i * Block().width) - hit.x)
                    xTemp = (i * Block().width) - hit.x;
            }

            if(yTemp > 0){
                if(yTemp > (j * Block().height) - hit.y - hit.h)
                    yTemp = (j * Block().height) - hit.y - hit.h;
            }
            else{
                if(yTemp < (j * Block().height) - hit.y)
                    yTemp = (j * Block().height) - hit.y;
            }
        }
    }


    speed_x = xTemp;
    speed_y = yTemp;
    move();
}


double Living :: distanceRight(Table table){
    double xTemp = width;
    double distTemp;

    for(int i = 0; i < table.width; i++) {
        distTemp = (i * Block().width) - (hit.x + hit.w);
        for(int j = 0; j < table.height; j++)
            if(xTemp != 0)
                if(*(table.blocks + i + (j * table.width)) != 0)
                    if((j * Block().height <= hit.y && hit.y < (j + 1) * Block().height)
                        || (j * Block().height < hit.y + hit.h && hit.y + hit.h < (j + 1) * Block().height))
                        if(distTemp >= 0)
                            if(distTemp < xTemp)
                                xTemp = distTemp;
    }

    return xTemp;
}

double Living :: distanceLeft(Table table){
    double xTemp = -width;
    double distTemp;

    for(int i = 0; i < table.width; i++) {
        distTemp = (i + 1) * Block().width - hit.x;
        for(int j = 0; j < table.height; j++)
            if(xTemp != 0)
                if(*(table.blocks + i + (j * table.width)) != 0)
                    if((j * Block().height <= hit.y && hit.y < (j + 1) * Block().height)
                        || (j * Block().height < hit.y + hit.h && hit.y + hit.h < (j + 1) * Block().height))
                        if(distTemp <= 0)
                            if(distTemp > xTemp)
                                xTemp = distTemp;
    }
    
    return xTemp;
}

double Living :: distanceDown(Table table){
    double yTemp = height;
    double distTemp;

    for(int j = 0; j < table.height; j++) {
        distTemp = j * Block().height - (hit.y + hit.h);
        for(int i = 0; i < table.width; i++)
            if(yTemp != 0)
                if(*(table.blocks + i + (j * table.width)) != 0)
                    if((i * Block().width <= hit.x && hit.x < (i + 1) * Block().width)
                        || (i * Block().width < hit.x + hit.w && hit.x + hit.w < (i + 1) * Block().width))
                        if(distTemp >= 0)
                            if(distTemp < yTemp)
                                yTemp = distTemp;
    }

    return yTemp;
}

double Living :: distanceUp(Table table){
    double yTemp = -height;
    double distTemp;

    for(int j = 0; j < table.height; j++) {
        distTemp = (j +1) * Block().height - hit.y;
        for(int i = 0; i < table.width; i++)
            if(yTemp != 0)
                if(*(table.blocks + i + (j * table.width)) != 0)
                    if((i * Block().width <= hit.x && hit.x < (i + 1) * Block().width)
                        || (i * Block().width < hit.x + hit.w && hit.x + hit.w < (i + 1) * Block().width))
                        if(distTemp <= 0)
                            if(distTemp > yTemp)
                                yTemp = distTemp;
    }

    return yTemp;
}


bool Living :: nextDiagonalBlock(Table table){
    return nextDiagonalBlock(table, speed_x > 0, speed_y > 0);
}

bool Living :: nextDiagonalBlock(Table table, bool x, bool y){
    int i, j;
    if(x){
        i = (hit.x + hit.w) / Block().width + 1;
    }
    else{
        i = (hit.x) / Block().width;
    }
    if(y){
        j = (hit.y + hit.h) / Block().height + 1;
    }
    else{
        j = (hit.y) / Block().height;
    }

    return *(table.blocks + i + (j * table.width)) != 0;
}