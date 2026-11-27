# NetWorQ

A terminal-based professional networking platform written in C++17. Create profiles, add skills and achievements, connect with other people, search by name, and find how two people are linked, all from a text menu.

The data structures the program is built on are written by hand in [`include/`](include): linked list, stack, queue, hash table, binary search tree and trie. The standard library is only used for plain helpers such as `std::vector`, `std::string`, `std::sort` and `std::hash`, plus `std::unordered_map` inside the trie nodes.

## Features

- **Profiles:** create a profile with a name, profession and experience. Each profile gets an ID, starting at 1001.
- **Skills and achievements:** add as many as you like to a profile.
- **Update profile:** change the name, profession and experience.
- **Browse:** list everyone alphabetically by name or grouped by profession.
- **Search:** find people by the start of their name. It is case-insensitive, so `al` finds `Aleen`.
- **Connections:** connect two users. The link works both ways, and the second person gets a notification.
- **Mutual connections:** see who two users both know.
- **Shortest path:** find the shortest chain of connections between two users.
- **Notifications:** process them one at a time, in the order they arrived.
- **Undo:** take back the latest skill added, achievement added or profile update.

## Build and run

You need a C++17 compiler (`c++`, which is `clang` or `g++`) and `make`.

```bash
git clone https://github.com/bhavyasingh-sa/NetWorQ.git
cd NetWorQ
make
./networq
```

The build uses `-Wall -Wextra -Wpedantic` and compiles without warnings. To remove the program:

```bash
make clean
```

## How to use it

When it starts, three sample profiles are loaded (Suhan, Aleen and Aditi, with IDs 1001 to 1003), and you get this menu:

```text
=== NetWorQ | Professional Networking ===
1. Create profile
2. List profiles alphabetically
3. List profiles by profession
4. Search profiles
5. View profile
6. Update profile
7. Add skill
8. Add achievement
9. Connect users
10. Mutual connections
11. Find shortest connection path
12. Process next notification
13. Undo latest action
0. Exit
```

Type the number and press Enter. The program then asks for what it needs, such as a user ID or a name. Mistakes (like an unknown ID) show an `Error:` message and return you to the menu.

### Example

Connect Suhan (1001) to Aleen (1002) and Aleen to Aditi (1003). Then ask for their mutual connections and the shortest path between Suhan and Aditi, add a skill to Suhan, undo it, and process the notifications. The program prints this (menus and prompts left out, and the profile shown after adding the skill):

```text
Connection created.
Connection created.
Mutual connections: Aleen
Path: Suhan -> Aleen -> Aditi

[1001] Suhan - Software Engineer
Experience: 3 years of experience
Skills: C++ (Advanced)
Achievements:
Connections: 1
Undid the latest skill addition.

To Aleen: Suhan connected with you.
To Aditi: Aleen connected with you.
```

## How it works

| Feature | Data structure | Where it is used |
| --- | --- | --- |
| Finding a user by ID | `HashTable` (separate chaining, 64 buckets) | `Network` keeps every user in a table of ID to profile |
| Skills, achievements, connections | `LinkedList` (singly linked, with head and tail) | One list of each per user, and the buckets of the hash table |
| Sorted listings | `BST` (binary search tree with a custom comparator) | Built when you list by name or by profession, then read in order |
| Name search | `Trie` (prefix tree over lowercase names) | Prefix lookup; rebuilt when a name changes |
| Notifications | `Queue` (FIFO, linked) | First notification in is the first one processed |
| Undo | `Stack` (LIFO, linked) | Each undoable action is pushed, and undo pops the latest |
| Shortest path | Breadth-first search using `Queue` and `HashTable` | The hash table remembers how each user was reached |

### Things to know

- **Nothing is saved.** All data lives in memory, so it is gone when you exit. The three sample profiles are created fresh on every start.
- **Undo covers three actions:** adding a skill, adding an achievement and updating a profile. Creating a profile or a connection cannot be undone.
- **Notifications:** connecting A to B queues a notification for B only.
- The binary search tree is a plain one, with no balancing.

## Project structure

```text
NetWorQ/
├── Makefile
├── include/
│   ├── BST.h / BST.tpp              binary search tree
│   ├── HashTable.h / HashTable.tpp  hash table
│   ├── LinkedList.h / LinkedList.tpp
│   ├── Queue.h / Queue.tpp
│   ├── Stack.h / Stack.tpp
│   └── Trie.h                       prefix tree (header only)
└── src/
    └── main.cpp                     the Network class, the menu and the program entry
```

## Possible improvements

- Save and load profiles from a file.
- Delete a profile.
- Search by profession or skill, not only by name.
- Balance the binary search tree.
- Add automated tests for the data structures.
