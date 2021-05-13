#include <mulib/muarg.h>

struct muarg_argument_config *
find_argument_from_name(struct muarg_header *info, const char *name)
{
	for (size_t i = 0; i < info->argument_count; i++)
	{
		if (strcmp(info->argument_list[i].name, name) == 0)
		{
			return &info->argument_list[i];
		}
	}

	return NULL;
}

struct muarg_argument_config *
find_argument_from_short_name(struct muarg_header *info, const char *name)
{
	for (size_t i = 0; i < info->argument_count; i++)
	{
		if (info->argument_list[i].short_name != NULL &&
			strcmp(info->argument_list[i].short_name, name) == 0)
		{
			return &info->argument_list[i];
		}
	}

	return NULL;
}

bool
is_a_valid_result(const char *str, struct muarg_argument_config *argument)
{
	if (argument->flag & ARG_FLAG_USE_ONLY_POSSIBLE_RESULT)
	{
		for (size_t i = 0; i < argument->arg_enum_count; i++)
		{
			if (strcmp(str, argument->arg_enum[i]) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

/* add string argument to the last entry of muarg_result.option_list */
void
parse_string_argument(struct muarg_result *info, int argv_id,
					  struct muarg_argument_config *argument)
{
	char *next_argument = NULL;

	if (info->raw_argument_count > argv_id + 1)
	{
		next_argument = info->raw_arguments[argv_id + 1];
	}

	if (next_argument == NULL)
	{
		printf("parameter: %s require a string argument \n", argument->name);
		exit(1);
	}

	argument->status.input = next_argument;

	if (argument->flag & ARG_FLAG_USE_ONLY_POSSIBLE_RESULT)
	{
		if (is_a_valid_result(next_argument, argument))
		{
			return;
		}

		printf(
			"%s is not a valid result for %s, must use these possible results:",
			next_argument, argument->name);

		muarg_show_help_option_possible_results(argument);
		printf("\n");
		exit(1);
	}
}

void
parse_string_value(struct muarg_result *final, int argv_id)
{
	int res = vec_push(&final->string_list, final->raw_arguments[argv_id]);
	if (res != 0)
	{
		exit(1);
	}
}

void
parse_single_argument(struct muarg_result *result, int argv_id,
					  struct muarg_header *option)
{
	char *current_argv = result->raw_arguments[argv_id];
	struct muarg_argument_config *argument = NULL;

	if (strncmp("--", current_argv, 2) == 0)	// long name
	{
		argument = find_argument_from_name(option, current_argv + 2);
	}
	else if (strncmp("-", current_argv, 1) == 0)	// short name
	{
		argument = find_argument_from_short_name(option, current_argv + 1);
	}
	else
	{
		parse_string_value(result, argv_id);
		return;
	}

	if (result == NULL)
	{
		printf("unknown argument: %s \n", current_argv);
		exit(-1);
	}

	argument->status.is_called = true;
	if ((argument->flag & ARG_FLAG_STRING ||
		 argument->flag & ARG_FLAG_USE_ONLY_POSSIBLE_RESULT))
	{
		parse_string_argument(result, argv_id, argument);
	}
	if (argument->callback != NULL)
	{
		argument->callback(option);
	}
}

struct muarg_result
muarg_eval(struct muarg_header *info, int argc, char **argv)
{
	struct muarg_result result;
	result.raw_argument_count = argc;
	result.raw_arguments = argv;
	result.argument_count = info->argument_count;
	result.argument_list = info->argument_list;

	vec_init(&result.string_list);
	for (int i = 1; i < argc; i++)
	{
		parse_single_argument(&result, i, info);
	}

	return result;
}

void
muarg_destroy_arg_result(struct muarg_result *result)
{
	vec_deinit(&result->string_list);
}

/* output: {element_1|element_2|element_3} for example */
void
muarg_show_help_option_possible_results(struct muarg_argument_config *option)
{
	printf("{");

	for (size_t i = 0; i < option->arg_enum_count; i++)
	{
		printf("%s", option->arg_enum[i]);
		if (i < option->arg_enum_count - 1)
		{
			printf("|");
		}
	}
	printf("}\n");
}

void
show_help_for_option(struct muarg_argument_config *option)
{
	if (option->help_msg == NULL)
	{
		return;
	}

	if (option->name != NULL)
	{
		printf("\t --%-10s", option->name);
	}
	else if (option->short_name != NULL)
	{
		printf("\t -%-10s", option->short_name);
	}

	if (option->flag & ARG_FLAG_STRING)
	{
		printf("%-5s", "{...}");
	}
	else if (option->flag & ARG_FLAG_USE_ONLY_POSSIBLE_RESULT)
	{
		muarg_show_help_option_possible_results(option);
		printf("\t%-15s\t  ", "");	  // realign everything
	}
	else
	{
		printf("%-5s", "");
	}

	printf(" %s \n", option->help_msg);
}

void
muarg_show_help(struct muarg_header *info)
{
	if (info->usage != NULL)
	{
		printf("Usage: %s %s \n", info->app_name, info->usage);
	}
	if (info->argument_count != 0)
	{

		printf("Options: \n");

		for (size_t i = 0; i < info->argument_count; i++)
		{
			show_help_for_option(&info->argument_list[i]);
		}
	}

	/* display even more information */
	if (info->help_info != NULL)
	{
		printf("\n %s \n", info->help_info);
	}
}

void
muarg_show_version(struct muarg_header *info)
{
	printf("%s version %s \n", info->app_name, info->version);
}

struct muarg_argument_status *
muarg_status_from_name(struct muarg_result *result, const char *name)
{
	for (size_t i = 0; i < result->argument_count; i++)
	{
		if (strcmp(result->argument_list[i].name, name) == 0)
		{
			return &result->argument_list[i].status;
		}
	}
	return NULL;
}

struct muarg_argument_status *
muarg_status_from_short_name(struct muarg_result *result,
							 const char *short_name)
{
	for (size_t i = 0; i < result->argument_count; i++)
	{
		if (strcmp(result->argument_list[i].short_name, short_name) == 0)
		{
			return &result->argument_list[i].status;
		}
	}
	return NULL;
}
