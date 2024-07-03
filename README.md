# Seat Reservation Project

This program is a basic command line interface with basic account and session system.

## Features
1. Seat reservation for different planes
2. Basic (and insecure) account and session system

## Usage
```
--help
```
For list of commands
```
--register
```
Interface for registration, saves to `accounts.dat`. Take note of your accountID after registration
```
--login [accountID] [password]
```
Logs in user and saves session to `session.dat`.
```
--logout
```
Logs out user and removes `session.dat`.
```
--create [filename]
```
Creates a new plane with specified filename
```
--display [filename]
```
Shows available and filled seats of the specific plane
```
--go-to-seat [filename] [seat]
```
Moves current user to seat of specific plane.
```
--clear-current-seat
```
Clears current seat of the user of specific plane.
```
--move-to-seat
```
Moves the current registered seat of the user of specific plane.

```
--view-account-information
```
Shows details of user information

```
--change-acount-name [first] [lastname]
```
Changes user name

```
--change-account-password [old] [new]
```
Changes user password

```
--change-account-age [age]
```
Changes user age

## Build
### Using CMake
1. Generate CMake files
```
cmake ./
```
3. Build make files
```
make
```

### Using GCC
1. Compile all source files
```
gcc main.c plane.c session.c account.c input-handler.c
```
2. Run application
```
./a.out
```
