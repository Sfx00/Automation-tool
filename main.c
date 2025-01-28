#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "libft.h"


void error(char *target)
{
    write(2,"No rule to make target '",24);
    write(2,target,ft_strlen(target));
    write(2,"'\n",2);
    exit(2);
}

void cmd_not_found(char *cmd)
{
    write(2,"Command not found : ",20);
    write(2,cmd,ft_strlen(cmd));
    write(2,"\n",1);
    exit(127);
}


size_t  f_depend(char * pos_d, char *full_file)
{
    int i;
    char **depends;
    char *depend;
    char *pos_end;

    i = 0;
    if(*pos_d == '\n')
        return(0);
    if(*pos_d == ' ')
        pos_d++;
    if(!ft_isalpha(*pos_d))
        return(0);
    pos_end = ft_strchr(pos_d,'\n');
    depend = ft_substr(pos_d,0,(pos_end - pos_d));
    depends = ft_split(depend,' ');


    while(depends[i])
    {
        printf("\n%s\n",depends[i]);
        excute_rules(depends[i],full_file);
        i++;
    }
    return((pos_end - pos_d) + 1);
}




void excute_rules(char *target,char *full_file)
{
    size_t size_target = ft_strlen(target);

    size_t size_full_file = ft_strlen(full_file);
    char *pos_target = ft_strnstr(full_file,target,size_full_file);
    if(!pos_target)
        error(target);
    size_t size_depend = f_depend(pos_target + (size_target + 1),full_file);

    char *pos_recipes = (pos_target + size_depend);
    char *end_recipes = ft_strchr(pos_recipes,'\n');
    if(!end_recipes)
        error(target);
    char *recipees = ft_substr(pos_recipes,0,(end_recipes - pos_recipes));
    char **recipe = ft_split(recipees,' ');
    if(!recipe || !*recipe)
        error(target);

    int pid;

    pid = fork();
    if(pid == 0)
    {
        execvp(recipe[0],recipe);
    }
    else
        waitpid(pid,NULL,0);
}







int main(int ac, char *av[], char *env[])
{
    char    *pos_recipes = NULL;
    char    *pos_target = NULL;
    char    *target;
    char    *buffer;
    char    *full_file = ft_strdup("");
    char    *end_recipes;
    char    **recipe;
    char    *full_path;
    int fd_m;
    int found_target = 0;
    int found_recipes = 0;
    size_t  i;
    size_t  size_buffer;
    size_t  size_target;
    size_t  size_full_file;
    
    
    if(!env) 
    {
        write(2,"empty environment variable\n",27);
        exit(2);
    }
    if(access("Makefile",F_OK) == -1 || access("Makefile",R_OK) == -1)
    {
        write(2,"Makefile not found or permission dinied\n",40);
        exit(2);
    }
    fd_m = open("Makefile",O_RDONLY);
    if(!fd_m)
    {
        write(2,"faild to found Makefile\n",19);
        exit(2);   
    }
    if(!av[1])
        target = "all:";
    else
        target = ft_strjoin(av[1],":");
    


    size_target = ft_strlen(target);

    while(1)
    {
        buffer = get_next_line(fd_m);
        if(!buffer)
            break;
        size_buffer = ft_strlen(buffer);
        full_file = ft_strjoin(full_file,buffer);
        if(ft_strnstr(buffer,target,size_buffer))
            found_target = 1;
        // if(ft_strnstr(buffer,"\t",size_buffer))
        //     found_recipes = 1;

    }
    if(found_target != 1)
        error(target);
    size_full_file = ft_strlen(full_file);
    pos_target = ft_strnstr(full_file,target,size_full_file);
    if(!pos_target)
        error(target);
    size_t size_depend = f_depend(pos_target + (size_target + 1),full_file);
    if(size_depend == 0)
        pos_recipes = (pos_target + (size_target + 2));
    else
        pos_recipes = (pos_target + size_depend);
    end_recipes = ft_strchr(pos_recipes,'\n');
    if(!end_recipes)
        error(target);
    char *recipees = ft_substr(pos_recipes,0,(end_recipes - pos_recipes));
    recipe = ft_split(recipees,' ');
    if(!recipe || !*recipe)
        error(target);

    // full_path = found_path(recipe[0],env);
    execvp(recipe[0],recipe);
    cmd_not_found(recipe[0]);
}









