#include "functions.h"

int main(int argc, char **argv) {

	int n, rank, size, new_rank;
	int count;
	unsigned char black[] = { 0, 0, 0 };

	MPI::Init(argc, argv);
	size = MPI::COMM_WORLD.Get_size();
	rank = MPI::COMM_WORLD.Get_rank();
	MPI_Status status ;
	MPI_Request request;

	int FstSkelSize;
	int* ranks1;
	int* ranks2;
	ranks1 = (int*)malloc((size/2)*sizeof(int));
	ranks2 = (int*)malloc((size/2)*sizeof(int));

	for (int i=0; i<size/2; i++){
		ranks1[i] = i;
		ranks2[i] = (size/2)+i;
	}

	MPI_Group orig_group, group_farm,group_map;
	MPI_Comm new_comm,comm_farm;

	/* Extract the original group handle */

	MPI_Comm_group(MPI_COMM_WORLD, &orig_group);


	/* Divide tasks into two distinct groups based upon rank */

	FstSkelSize = size/2;
	if (rank < size/2) { MPI_Group_incl(orig_group, size/2, ranks1, &group_map);}
	else { MPI_Group_incl(orig_group, size/2, ranks2, &group_map); }

	MPI_Comm_create(MPI_COMM_WORLD, group_map, &new_comm);

	MPI_Group_rank (group_map, &new_rank);

	cout << "myRank: " << rank <<" myNewRank: "<< new_rank << endl;

	unsigned char** MatrixR;
	unsigned char** MatrixG;
	unsigned char** MatrixB;

	int height,width =0;

	//parameters for the scatter
	int * displ;
	int *scounts;
	displ = (int*)malloc((size-FstSkelSize-1)*sizeof(int));
	scounts = (int*)malloc((size-FstSkelSize-1)*sizeof(int));


	double x,y;
	double xstart,xstep,ystart,ystep;
	double xend, yend;
	double z,zi,newz,newzi;
	double colour;
	int iter,dest,source;
	long col;
	int i,j,k;
	int inset;
	int fd;
	int neg;

	int EOS = 0;

	//stream process for Mandelbrot farm
	if (rank == 0){

		for (int m=0; m<atoi(argv[1]);m++){

			/*Images have two possible sizes*/
			if (rand() > RAND_MAX/2){
				width = 800;
				height = 600;
			}
			else{
				width = 600;
				height = 480;
			}


			/*Generate randomly the extremes*/
			xstart = RandomGen(-2,2);
			//cout << "xstart " << xstart << endl;
			xend = RandomGen(xstart,2);
			//cout << "xend " << xend << endl;
			ystart = RandomGen(-2,2);
			yend = RandomGen(ystart,2);
			//			 all processes in a communicator
			//receive the rank of the destination
			MPI_Recv(&dest,1,MPI_INT,MPI_ANY_SOURCE,5,MPI_COMM_WORLD,&status);
			MPI_Send(&EOS,1, MPI_INT, dest, 5, MPI_COMM_WORLD);
			//send width height and parameters
			MPI_Send(&width,1, MPI_INT, dest, 5, MPI_COMM_WORLD);
			MPI_Send(&height,1, MPI_INT, dest, 5, MPI_COMM_WORLD);
			MPI_Send(&xstart,1, MPI_DOUBLE, dest, 5, MPI_COMM_WORLD);
			MPI_Send(&ystart,1, MPI_DOUBLE, dest, 5, MPI_COMM_WORLD);
			MPI_Send(&xend,1, MPI_DOUBLE, dest, 5, MPI_COMM_WORLD);
			MPI_Send(&yend,1, MPI_DOUBLE, dest, 5, MPI_COMM_WORLD);

		}
		EOS++;
		for (int i=0; i<FstSkelSize-1;i++){
			MPI_Recv(&dest,1,MPI_INT,MPI_ANY_SOURCE,5,MPI_COMM_WORLD,&status);
			MPI_Send(&EOS,1, MPI_INT, dest, 5, MPI_COMM_WORLD);
		}
	}


	if (rank > 0 && rank < FstSkelSize){
		iter = 200;

		while (1){
			//Send my rank for on demand farm
			MPI_Send(&rank,1, MPI_INT, 0, 5, MPI_COMM_WORLD);
			//cout << "rank sent "<< rank<<endl;
			MPI_Recv(&EOS,1,MPI_INT,0,5,MPI_COMM_WORLD,&status);
			//cout <<"EOS: " << EOS << endl;
			if (EOS != 0) break;
			MPI_Recv(&width,1,MPI_INT,0,5,MPI_COMM_WORLD,&status);
			MPI_Recv(&height,1,MPI_INT,0,5,MPI_COMM_WORLD,&status);
			MPI_Recv(&xstart,1,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
			MPI_Recv(&ystart,1,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
			MPI_Recv(&xend,1,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
			MPI_Recv(&yend,1,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);

			//Allocate space for matrixes
			malloc2duchar(&MatrixR,width,height);
			malloc2duchar(&MatrixG,width,height);
			malloc2duchar(&MatrixB,width,height);



			/* these are used for calculating the points corresponding to the pixels */
			xstep = (xend-xstart)/width;
			ystep = (yend-ystart)/height;

			/*the main loop */
			x = xstart;
			//cout << "xstart: "<<xstart<<endl;
			y = ystart;
			//cout << "ystart: "<<ystart<<endl;
			for (i=0; i<width; i++)
			{
				//printf("Now on line: %d\n", i);
				for (j=0; j<height; j++)
				{
					z = 0;
					zi = 0;
					inset = 1;
					for (k=0; k<iter; k++)
					{
						/* z^2 = (a+bi)(a+bi) = a^2 + 2abi - b^2 */
						newz = (z*z)-(zi*zi) + x;
						newzi = 2*z*zi + y;
						z = newz;
						zi = newzi;
						if(((z*z)+(zi*zi)) > 4)
						{
							inset = 0;
							colour = k;
							k = iter;
							break;
						}
					}
					if (inset)
					{
						MatrixR[i][j] = 255;
						MatrixG[i][j] = 255;
						MatrixB[i][j] = 255;
					}
					else
					{
						MatrixR[i][j] = sin(colour)*255;
						MatrixG[i][j] = cos(colour)*255;
						MatrixB[i][j] = (colour/iter)*255;
					}
					x += xstep;

				}
				y += ystep;

				x = xstart;
			}

			//pass the matrix to the scatterer
			MPI_Send(&rank,1, MPI_INT, FstSkelSize, 5, MPI_COMM_WORLD);
			MPI_Send(&EOS,1, MPI_INT, FstSkelSize, 5, MPI_COMM_WORLD);
			MPI_Send(&width,1, MPI_INT, FstSkelSize, 5, MPI_COMM_WORLD);
			MPI_Send(&height,1, MPI_INT, FstSkelSize, 5, MPI_COMM_WORLD);
			MPI_Send(&(MatrixR[0][0]),width*height, MPI_UNSIGNED_CHAR, FstSkelSize,15, MPI_COMM_WORLD);
			MPI_Send(&(MatrixG[0][0]),width*height, MPI_UNSIGNED_CHAR, FstSkelSize,15, MPI_COMM_WORLD);
			MPI_Send(&(MatrixB[0][0]),width*height, MPI_UNSIGNED_CHAR, FstSkelSize,15, MPI_COMM_WORLD);
			//free the matrixes
			free2duchar(&MatrixB);
			free2duchar(&MatrixG);
			free2duchar(&MatrixR);

		}
		MPI_Send(&rank,1, MPI_INT, FstSkelSize, 5, MPI_COMM_WORLD);
		MPI_Send(&EOS,1, MPI_INT, FstSkelSize, 5, MPI_COMM_WORLD);

		//	cout << "end of " << rank << endl;

	}

	double widthLocal,widthBound;
	unsigned char *BufferR;
	unsigned char *BufferG;
	unsigned char *BufferB;
	int myWidth;

	// in this case I am the scatterer
	if (rank == FstSkelSize){
		int oldwidth=width;
		int oldheight=height;
		int count =0;

		while(count < rank-1){
			//receive the end of stream
			MPI_Recv(&source,1,MPI_INT,MPI_ANY_SOURCE,5,MPI_COMM_WORLD,&status);
			MPI_Recv(&EOS,1,MPI_INT,source,5,MPI_COMM_WORLD,&status);
			//cout << rank << " " << "EOS: "<< EOS << endl;
			if (EOS == 0){
				MPI_Recv(&width,1,MPI_INT,source,5,MPI_COMM_WORLD,&status);
				MPI_Recv(&height,1,MPI_INT,source,5,MPI_COMM_WORLD,&status);

				widthBound = floor( (double)width / (double)(size-FstSkelSize-1) );
				cout << widthBound*height << endl;

				if (oldwidth != width || oldheight != height){
					if (oldwidth > 0){
						free2duchar(&MatrixB);
						free2duchar(&MatrixG);
						free2duchar(&MatrixR);}
					//Allocate space for matrixes
					malloc2duchar(&MatrixR,width,height);
					malloc2duchar(&MatrixG,width,height);
					malloc2duchar(&MatrixB,width,height);
				}


				for (int dest = 1; dest <= (size-FstSkelSize-1); dest++)
				{
					scounts[dest-1] = widthBound;
					displ[dest-1] = 0;
					if (dest == size-FstSkelSize-1 ) {
						cout <<"ciao" <<endl;
						scounts[dest-1] = scounts[dest-1] +(width % (size-FstSkelSize-1));
					}
					scounts[dest-1] = scounts [dest-1]*height;
					if (dest > 1)
						MPI_Send(&(scounts[dest-1]),1, MPI_INT, rank+dest-1, 5, MPI_COMM_WORLD);
					else
						myWidth=scounts[dest-1];
				}

				cout << "rank " << rank << " mywidth "<< scounts[0] <<endl;
				MPI_Recv(&(MatrixR[0][0]),width*height,MPI_UNSIGNED_CHAR,source,15,MPI_COMM_WORLD,&status);
				MPI_Recv(&(MatrixG[0][0]),width*height,MPI_UNSIGNED_CHAR,source,15,MPI_COMM_WORLD,&status);
				MPI_Recv(&(MatrixB[0][0]),width*height,MPI_UNSIGNED_CHAR,source,15,MPI_COMM_WORLD,&status);

			}
			else count++;
		}
	}


	if (rank > FstSkelSize && rank < size-1){
		MPI_Recv(&myWidth,1,MPI_INT,FstSkelSize,5,MPI_COMM_WORLD,&status);
		cout << rank << " " << myWidth << endl;
		/*//Do computations on sub-image
						CImg<unsigned char> oImage(widthLocal,height,1,3,0);


						for(int x = 0; x < widthLocal; x++)
						{
							for (int y = 0; y < height; y++)
							{
								oImage.draw_point(x, y, black);
								const unsigned char color[] = { MatrixR[x][y], MatrixG[x][y],MatrixB[x][y] };
								oImage.draw_point(x, y, color);
							}
						}
						char a[16]; int n;
						n = sprintf(a,"%d.png",(rank+count)*(count+1));
						count++;
						//oImage.save(a);*/
	}

	if(rank >= FstSkelSize && rank < size-1){
		BufferR = (unsigned char*)malloc(myWidth*sizeof(unsigned char));
		MPI_Scatterv(&(MatrixR[0][0]),scounts,displ,MPI_UNSIGNED_CHAR,BufferR,myWidth,MPI_UNSIGNED_CHAR,0,new_comm);
		//	MPI_Scatterv(&(MatrixG[0][0]),scounts,displ,MPI_UNSIGNED_CHAR,BufferG,widthBound*height,MPI_UNSIGNED_CHAR,0,new_comm);
		//	MPI_Scatterv(&(MatrixB[0][0]),scounts,displ,MPI_UNSIGNED_CHAR,BufferB,widthBound*height,MPI_UNSIGNED_CHAR,0,new_comm);

	}




	MPI::Finalize();
	exit(0);



}
