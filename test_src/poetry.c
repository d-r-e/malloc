#include "../inc/ft_malloc.h"
#include "../libft/libft.h"

int main(void) {
	char *poetry =
			"Lorem Ipsum es simplemente el texto de relleno de las \
    imprentas y archivos de texto. Lorem Ipsum ha sido el texto de relleno \
    estándar de las industrias desde el año 1500, cuando un impresor (N. del\
     T. persona que se dedica a la imprenta) desconocido usó una galería de \
     textos y los mezcló de tal manera que logró hacer un libro de textos \
     especimen. No sólo sobrevivió 500 años, sino que tambien ingresó como\
     texto de relleno en documentos electrónicos, quedando esencialmente igual \
     al original. Fue popularizado en los 60s con la creación de las hojas \"\
      Letraset \", las cuales contenian pasajes de Lorem Ipsum, y más recientemente\
       con software de autoedición, como por ejemplo Aldus PageMaker, el cual \
       incluye versiones de Lorem Ipsum.";

	char **spl;
	char **modified_spl;
	char **tmp;

	spl = ft_split(poetry, ' ');

	size_t count = 0;
	tmp = spl;
	while (tmp && *tmp) {
		count++;
		tmp++;
	}

	modified_spl = (char **) malloc((count + 1) * sizeof(char *));
	for (size_t i = 0; i < count; i++) {
		size_t len = strlen(spl[i]);
		modified_spl[i] = (char *) malloc(len + 2);
		ft_strcpy(modified_spl[i], spl[i]);
		modified_spl[i][len] = ' ';
		modified_spl[i][len + 1] = '\0';
	}
	modified_spl[count] = NULL;
	show_alloc_mem();

	tmp = spl;
	char **tmp_mod = modified_spl;
	while (tmp && *tmp && tmp_mod && *tmp_mod) {
		printf("Original: %s, Modified: %s\n", *tmp, *tmp_mod);

		if (ft_strcmp(*tmp, *tmp_mod) == 0) {
			printf("Strings are the same, but they shouldn't be!\n");
		}
		tmp++;
		tmp_mod++;
	}

	// Cleanup
	tmp = spl;
	while (tmp && *tmp) {
		free(*tmp);
		tmp++;
	}
	free(spl);

	tmp_mod = modified_spl;
	while (tmp_mod && *tmp_mod) {
		free(*tmp_mod);
		tmp_mod++;
	}
	free(modified_spl);
	//    show_alloc_mem();
}