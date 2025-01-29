#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "libft.h"


int flag = 1;

void error(char *target, int flags)
{
    write(2,"No rule to make target '",24);
    write(2,target,ft_strlen(target));
    write(2,"'\n",2);
    if(flags == 1)
        exit(2);
}

void cmd_not_found(char *cmd)
{
    write(2,"Command not found : ",20);
    write(2,cmd,ft_strlen(cmd));
    write(2,"\n",1);
    exit(127);
}


int excute_rules(char *action, char *full_file)
{
    char *pos_end;
    char    *cmd;
    char    **full_cmd;
    int     pid;

    if(!flag)
        return(-1);

    pos_end = ft_strchr(action,'\n');
    cmd = ft_substr(action,0,(pos_end - action));
    full_cmd = ft_split(cmd,' ');
    pid = fork();
    if(pid == 0)
        flag = execvp(full_cmd[0],full_cmd);
    else
        waitpid(pid,NULL,0);
    return(flag);
}



int target_action(char *name, char *full_file, size_t size_full_file)
{
    char    *pos_target;
    char    *pos_new_line;
    char    *rule_line;
    char    **rule;
    int i;
    char    *target;

    
    if(!flag)
        return(-1);

    target = ft_strjoin(name,":");
    pos_target = ft_strnstr(full_file,target,size_full_file);
    if(!pos_target)
    {
        error(target,0);
        return(-1);
    }
    pos_new_line = ft_strchr(pos_target,'\n');
    rule_line = ft_substr(pos_target,0,(pos_new_line - pos_target));
    rule = ft_split(rule_line,' ');
    if(!rule)
        return(-1);

    i = 1;
  
    while(rule[i] != NULL)
    {
        flag = target_action(rule[i],full_file,size_full_file);
        if(!flag)
            return(-1);
        i++;
    }
    flag = excute_rules(pos_new_line + 2,full_file);
    return(flag);
}









int main(int ac, char *av[], char *env[])
{
    char    *pos_recipes = NULL;
    char    *pos_target = NULL;
    char    *target;
    char    *buffer;
    char    *full_file = ft_strdup("");
    char    *end_recipes;
    char    **rule;
    char    *full_path;
    char    *line;
    char    *new_line;
    char    **recipe;
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
        error(target,1);
    size_full_file = ft_strlen(full_file);


  
    pos_target = ft_strnstr(full_file,target,size_full_file);
    if(!pos_target)
        error(target,1);
    new_line = ft_strchr(pos_target,'\n');
    line = ft_substr(pos_target,0,(new_line - pos_target));
    rule = ft_split(line,' ');
    if(!rule)
        error(target,1);

    i = 1;
    while(rule[i] != NULL)
    {
        flag = target_action(rule[i],full_file,size_full_file);
        if(flag == -1)
            error(target,1);
        i++;
     }

    pos_recipes = new_line + 2;
    end_recipes = ft_strchr(pos_recipes,'\n');
    if(!end_recipes)
        error(target,1);
    char *recipees = ft_substr(pos_recipes,0,(end_recipes - pos_recipes));
    recipe = ft_split(recipees,' ');
    if(!recipe || !*recipe)
        error(target,1);
    // full_path = found_path(recipe[0],env);
    execvp(recipe[0],recipe);
    cmd_not_found(recipe[0]);
}









