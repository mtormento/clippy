# Why?
Has ever happened to you when you are setting up your new smartphone and you are asked for the wifi password to feel the extreme urge to start cursing every known divinity?
Clippy solves this problem by turning your Flipper Zero into a cross-device shared clipboard.

# How does it work?
Pretty simple, Clippy has 2 modes:
- copy
  - turns Flipper Zero into a mass storage device
  - you'll find a file named clipboard.txt at the root of the filesystem
  - you can store a list of strings there along with a description
  - you can create different files if you want to organise your clipboards
- paste
  - turns Flipper Zero into a usb/bt keyboard
  - you can choose from clipboard items built from the list of strings gathered from clipboard.txt
  - selecting one paste the string to the connected device
