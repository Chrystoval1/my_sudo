# my_sudo Project

## Overview  
**my_sudo** is a custom implementation of the `sudo` command in C. This project aims to replicate the basic functionality of `sudo`, providing user access control based on configurations from the `/etc/sudoers` file. The project is designed to offer flexible user and group management with additional flags and security policies.

> **Important**:  
> This project doesn't support the full configuration of the `sudoers` file. It only handles certain features that have been planned and implemented.

## Planned Features

### Flags

The following flags are planned and implemented:

- [x] **-u <user>**: Execute the command as the specified user.
- [x] **-u #<uid_user>**: Execute the command as the user identified by the specified UID.
- [x] **-g <group>**: Execute the command with the permissions of the specified group.
- [x] **-g #<gid_group>**: Execute the command with the permissions of the group identified by the specified GID.
- [x] **-s**: Run the command in the user's shell.
- [x] **-s + <command>**: Run a specified command in the user's shell.
- [x] **-E**: Preserve the environment variables when running the command.
- [x] **-l**: List the user's sudo privileges.

### Security Policy

The following security checks are planned and implemented to ensure controlled execution and compliance with sudoers configuration:

- [x] **Check password 3 times**: The user is prompted for a password up to three times before access is denied.
- [x] **Check sudoers file for user specification**: Validate if the user is listed in the `sudoers` file and authorized for the requested command.
- [x] **Check sudoers file for group specification**: Validate if the user's group is listed in the `sudoers` file with the appropriate permissions.
- [x] **Check sudoers file for user with `#`**: Check if the user has a specific configuration entry using their UID (indicated by `#`).
- [x] **Check sudoers file for group with `#`**: Check if the group is specified with a GID (indicated by `#`) in the `sudoers` file.

## Limitations

- Currently, **my_sudo** does not support the full range of configurations available in the `sudoers` file.
- Only a subset of flags and security policies have been implemented.

## Usage

To run **my_sudo**, use the following command format:

`my_sudo [flags] <command>`

For example:

- Run a command as a specific user:  
  `my_sudo -u <user> <command>`
  
- Run a command as a specific group:  
  `my_sudo -g <group> <command>`

- Preserve the environment while running a command:  
  `my_sudo -E <command>`

### Example

To run the `ls` command as user `john`:

`my_sudo -u john ls`

## Contributing

If you want to contribute to the project:
1. Fork the repository.
2. Create a new branch for your feature or fix.
3. Submit a pull request with your changes.
