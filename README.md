# minishell

`minishell` is literally mini shell. With `fork` and `execve` system call, minishell run program inside shell process.
`fork`ed process is child process which copy memory and CPU register of parent(`minishell`). After `execve` call, child process overwrite its own memory space with new program data and run program like normal.

<br/><br/>

## Requirements

`minishell` uses [libft](https://github.com/jebae/libft), [ft_printf](https://github.com/jebae/ft_printf.git).

```
# download libft
git clone https://github.com/jebae/libft.git

# download ft_printf
git clone https://github.com/jebae/ft_printf.git

# directory structure
root/
 libs/
  libft
  ft_print
```

<br/><br/>

## Install

```
make
```

<br/><br/>

## Usage

```
./minishell
$>
```

<br/><br/>

## Features

### builtin command

- `cd`
- `echo`
- `env`: show environment variable.
- `setenv`: upsert environment variable.
	e.g. `setenv foo=bar`
- `unsetenv`: unset environment variable.
	e.g. `setenv foo`
- `dirs`: show directory visit history.
	```
	$> dirs
	0 /Users/jebae/Documents/dev/42
	1 /Users/jebae/Documents/dev
	2 /Users/jebae
	3 /Users/jebae/Documents
	4 /Users/jebae/Documents/dev/42/minishell
	```
	history stack is ordered most recent visit first.

### special expression

- `$`: replace word after `$` with env if exists.
- `~`
	- `~`: show `$HOME`
	- `~+`: show current directory
	- `~-`: show `OLDPWD`
	- `~[-+][0-9]+`: show directory history entry from history stack.
		e.g. `~-1` means last entry in history stack.
- `"`: protect words enclosed between `"`. Variable substitution with `$` is possible.
- `'`: protect words enclosed between `"`. Variable substitution with `$` is impossible.
- `\`: escape special expression character.
