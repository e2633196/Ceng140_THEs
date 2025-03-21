#include <stdio.h>
#include <stdlib.h>

int transform_name(const char *name)
{
    int i,sum=0;
    for(i=0; name[i]!='\0'; i++)
    {

        sum+=(i+1)*(i+1)*name[i];
    }

    return sum;
}

int** initialize_storage(int IA_size, int DA_size, int** DA_sizes)
{
    int **storage, *new_array;
    int l,k;
    new_array=(int*)calloc(IA_size,sizeof(int));
    storage=(int**)calloc(IA_size,sizeof(int*));

    for(l=0; l<IA_size; l++)
    {
        storage[l]=(int*)calloc(DA_size,sizeof(int));

    }

    *DA_sizes=new_array;

    for(k=0; k<IA_size; k++)
    {
        new_array[k]=DA_size;
    }

    return storage;

}

void print_storage(int** storage, int IA_size, int* DA_sizes)
{
    int outfor, infor,var_print;
    for(outfor=0; outfor<IA_size ; outfor++)
{
printf("%d -> ",outfor);
for(infor=0; infor<(DA_sizes[outfor]-1); infor++)
{
var_print=storage[outfor][infor];
printf("%d ", var_print); 
}
var_print=storage[outfor][infor];

printf("%d\n", var_print);
}
}

void insert(int** storage, int IA_size, const char* name, int* DA_sizes)
{
    int loop, new_index;
    int location_empty_index, control;
    int corresponding_index, name_code;
    int *new_pointer ;

    name_code=transform_name(name);
    corresponding_index=name_code%IA_size;

    for(location_empty_index=0; location_empty_index<DA_sizes[corresponding_index]; location_empty_index++)
    {
        control=0;

        if(storage[corresponding_index][location_empty_index])
            continue;

        else
        {
            control=1;
            storage[corresponding_index][location_empty_index]=name_code;
            break;
        }
    }

    if(!control)
    {
        new_pointer=(int*)realloc(storage[corresponding_index],DA_sizes[corresponding_index]*2*sizeof(int));

        if(new_pointer)
        {
            for(loop=DA_sizes[corresponding_index]; loop<(DA_sizes[corresponding_index]*2); loop++)
            {
                new_pointer[loop]=0;
            }
            new_index=DA_sizes[corresponding_index];
            new_pointer[new_index]=name_code;


            storage[corresponding_index]=new_pointer;

            DA_sizes[corresponding_index]*=2;
        }
    }   
}

void insert2(int** storage, int IA_size, int name_code, int* DA_sizes)
{

    int loop, new_index;
    int location_empty_index, control;
    int corresponding_index;
    int *new_pointer ;


    corresponding_index=name_code%IA_size;

    for(location_empty_index=0; location_empty_index<DA_sizes[corresponding_index]; location_empty_index++)
    {
        control=0;

        if(storage[corresponding_index][location_empty_index])
            continue;

        else
        {
            control=1;
            storage[corresponding_index][location_empty_index]=name_code;
            break;
        }
    }

    if(!control)
    {
        new_pointer=(int*)realloc(storage[corresponding_index],DA_sizes[corresponding_index]*2*sizeof(int));

        if(new_pointer)
        {
            for(loop=DA_sizes[corresponding_index]; loop<(DA_sizes[corresponding_index]*2); loop++)
            {
                new_pointer[loop]=0;
            }
            new_index=DA_sizes[corresponding_index];
            new_pointer[new_index]=name_code;


            storage[corresponding_index]=new_pointer;

            DA_sizes[corresponding_index]*=2;
        }
    } 

}

void fill_new_storage(int** storage, int IA_size, int** new_storage, int *DA_sizes, int* new_DA_sizes)
{
    int outerfor, insidefor, new_IA_size=IA_size*2;



    for(outerfor=0; outerfor<IA_size; outerfor++)
    {
        for(insidefor=0; insidefor<DA_sizes[outerfor]; insidefor++)
        {
            if(storage[outerfor][insidefor])
            insert2(new_storage, new_IA_size, storage[outerfor][insidefor], new_DA_sizes);

        }
    }
}

void resize_IA(int*** storage, int* IA_size, int DA_size, int** DA_sizes)
{

    int extentioncontrol, sumofdataarrays=0, extension_condition, loop;
    int *old_DAsizes, new_IA_size=2*(*IA_size);
    int **new_indexarray;

    extension_condition=(*IA_size)*DA_size*1.5;
    old_DAsizes=*DA_sizes;




    for(extentioncontrol=0 ; extentioncontrol<(*IA_size); extentioncontrol++)
    {
        sumofdataarrays+=(*DA_sizes)[extentioncontrol];
    }

    if(sumofdataarrays>=extension_condition)
    {
        int *new_DAsizes;

        new_indexarray=initialize_storage(new_IA_size, DA_size, &new_DAsizes);

        fill_new_storage(*storage, *IA_size, new_indexarray,old_DAsizes, new_DAsizes);




        for(loop=0; loop<*IA_size; loop++)
        {

            free(*((*storage)+loop));
        }
        free(*storage);
        *storage=new_indexarray;


        free(old_DAsizes);
        *DA_sizes=new_DAsizes;
        *IA_size*=2;
    }
}