#include "Image.h"


int main(int argc, char** argv) {
	
	printf("\n\e[0;33m##########################################\e[0m\n");
	printf("\e[0;33m########## \e[1;35m Image Processing \e[0;33m ############\e[0m\n");
	printf("\e[0;33m##########################################\e[0m\n\n");

    printf("Enter the full path of your image:\n");
    char input[512];
    scanf("%s", input);
    Image usr_img(input);
    Image copy(usr_img);


    printf("\nChoose one of the following functions:\n\n");

	printf("\e[1;36m 1\e[0m -> Crop Image.\n");
    printf("\e[1;36m 2\e[0m -> Copy Area.\n");
    printf("\e[1;36m 3\e[0m -> Mask Color.\n");
    printf("\e[1;36m 4\e[0m -> Change Pixel Color.\n");
    printf("\e[1;36m 5\e[0m -> Draw a frame.\n");
    printf("\e[1;36m 6\e[0m -> Find a Color.\n");
    printf("\e[1;36m 7\e[0m -> Exit.\n\n");

    bool success;
    int choice;
    uint16_t cx, cy, cw, ch, to_x, to_y;
    enum mode {BUFFER, CROP_IMG, COPY_AREA, MASK_COLOR, CHANGE_PIXEL, FRAME, FIND_COLOR, EXIT};

    begin:
        printf("Your Choice: ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
            case CROP_IMG:
            	usr_img.print_img_info();

            	printf("Enter the X and Y of the starting position: ");
        		scanf("%hu %hu", &cx, &cy);
        		printf("Enter the Width and Height of the desired Image: ");
        		scanf("%hu %hu", &cw, &ch);

            	copy.crop(cx, cy, cw, ch);

            	success = copy.write("result.png");
            	if(success){
            		printf("\e[0;32mCommand Executed Successfully\e[0m\n");
            	}
                break;

            case COPY_AREA:
            	usr_img.print_img_info();

            	printf("Enter the X and Y of the starting position: ");
        		scanf("%hu %hu", &cx, &cy);
        		printf("Enter the Width and Height of the desired Image: ");
        		scanf("%hu %hu", &cw, &ch);
        		printf("Enter the X and Y of the desired position: ");
        		scanf("%hu %hu", &to_x, &to_y);

            	copy.copy_area(cx, cy, cw, ch, to_x, to_y);

            	success = copy.write("result.png");
            	if(success){
            		printf("\e[0;32mCommand Executed Successfully\e[0m\n");
            	}
                break;

            case MASK_COLOR:
            	float r, g, b;
            	printf("Enter R G B ratio values as float between\e[0;32m 0 and 1\e[0m: ");
        		scanf("%f %f %f", &r, &g, &b);

            	copy.color_mask(r, g, b);

            	success = copy.write("result.png");
            	if(success){
            		printf("\e[0;32mCommand Executed Successfully\e[0m\n");
            	}
                break;

            case CHANGE_PIXEL:
            	int f_r, f_g, f_b, to_r, to_g, to_b, threshold;
            	printf("Enter R G B value of the \e[0;31mOLD\e[0m Color: ");
        		scanf("%d %d %d", &f_r, &f_g, &f_b);
        		printf("Enter R G B value of the \e[0;32mNEW\e[0m Color: ");
        		scanf("%d %d %d", &to_r, &to_g, &to_b);
        		printf("Enter Threshold value (color sensitivity): ");
        		scanf("%d", &threshold);

            	copy.change_color(f_r, f_g, f_b, to_r, to_g, to_b, threshold);

            	success = copy.write("result.png");
            	if(success){
            		printf("\e[0;32mCommand Executed Successfully\e[0m\n");
            	}
                break;

            case FRAME:
            	int ptrn_num, frame_size, frame_r, frame_g, frame_b;
            	printf("Choose the shape of the Frame, Enter just the NUMBER:\n");
            	printf("1. Plus    2. Corners    3. Devil Eye    4. Flower\n");
            	printf("Your Choice: ");
        		scanf("%d", &ptrn_num);
        		if(ptrn_num > 4 || ptrn_num < 1){
        			ptrn_num = 1;
                    printf("Choosing defualt value: 1");
        		}

            	printf("\nChoose the size of the Frame, Enter just the NUMBER:\n");
            	printf("0. 50px    1. 100px    2. 200px    3. 300px\n");
            	printf("Your Choice: ");
        		scanf("%d", &frame_size);
        		if(frame_size > 3 || frame_size < 0){
        			frame_size = 1;
                    printf("Choosing defualt value: 1");
        		}

        		printf("\nEnter R G B value of Frame Color: ");
        		scanf("%d %d %d", &frame_r, &frame_g, &frame_b);

            	copy.frame(ptrn_num, frame_size, frame_r, frame_g, frame_b);

            	success = copy.write("result.png");
            	if(success){
            		printf("\e[0;32mCommand Executed Successfully\e[0m\n");
            	}
            	break;

            case FIND_COLOR:
                int srch_r, srch_g, srch_b, border_r, border_g, border_b, thickness;
                printf("Enter R G B value of the \e[0;31mSearched\e[0m Color: ");
                scanf("%d %d %d", &srch_r, &srch_g, &srch_b);
                printf("Enter R G B value of the \e[0;32mBorder\e[0m Color: ");
                scanf("%d %d %d", &border_r, &border_g, &border_b);
                printf("Enter Thickness value of the Border: ");
                scanf("%d", &thickness);

                copy.find_color( srch_r, srch_g,  srch_b,  border_r,  border_g,  border_b, thickness);
            	
                success = copy.write("results.png");
                if(success){
                    printf("\e[0;32mCommand Executed Successfully\e[0m\n");
                }
                break;

            case EXIT:
                break;

            default:
                printf("\nPlease enter a valid number !!\n");
                goto begin;
        }

	printf("\n///// PROGRAM ENDED /////\n\n");
	return 0;
}
