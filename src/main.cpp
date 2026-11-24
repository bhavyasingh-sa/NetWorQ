#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../include/BST.h"
#include "../include/HashTable.h"
#include "../include/LinkedList.h"
#include "../include/Queue.h"
#include "../include/Stack.h"
#include "../include/Trie.h"

struct Skill {
    std::string name;
    std::string level;
};

struct Achievement {
    std::string title;
    std::string description;
};

struct User {
    int id;
    std::string name;
    std::string profession;
    std::string experience;
    LinkedList<Skill> skills;
    LinkedList<Achievement> achievements;
    LinkedList<int> connections;

    User(int id, std::string name, std::string profession, std::string experience)
        : id(id),
          name(std::move(name)),
          profession(std::move(profession)),
          experience(std::move(experience)) {}
};

struct UserNameLess {
    bool operator()(const User* a, const User* b) const {
        if (a->name != b->name) {
            return a->name < b->name;
        }
        return a->id < b->id;
    }
};

struct UserProfessionLess {
    bool operator()(const User* a, const User* b) const {
        if (a->profession != b->profession) {
            return a->profession < b->profession;
        }
        if (a->name != b->name) {
            return a->name < b->name;
        }
        return a->id < b->id;
    }
};

struct Notification {
    int userId;
    std::string message;
};

enum class ActionType { AddSkill, AddAchievement, UpdateProfile };

struct HistoryEntry {
    ActionType type;
    int userId;
    std::string oldName;
    std::string oldProfession;
    std::string oldExperience;
};

class Network {
    HashTable<int, User*> users;
    Trie names;
    Stack<HistoryEntry> history;
    Queue<Notification> notifications;
    int nextId = 1001;

    void rebuildSearchIndex() {
        names.clear();
        for (User* u : users.values()) {
            names.insert(u->name, u->id);
        }
    }

public:
    ~Network() {
        for (User* u : users.values()) {
            delete u;
        }
    }

    User* addUser(const std::string& name, const std::string& profession,
                  const std::string& experience) {
        User* user = new User(nextId++, name, profession, experience);
        users.insert(user->id, user);
        names.insert(name, user->id);
        return user;
    }

    User* get(int id) {
        if (!users.contains(id)) {
            return nullptr;
        }
        return users.get(id);
    }

    void addSkill(int id, const std::string& skill, const std::string& level) {
        User* u = get(id);
        if (!u) {
            throw std::runtime_error("User ID not found.");
        }
        u->skills.insertEnd({skill, level});
        history.push({ActionType::AddSkill, id, "", "", ""});
    }

    void addAchievement(int id, const std::string& title, const std::string& description) {
        User* u = get(id);
        if (!u) {
            throw std::runtime_error("User ID not found.");
        }
        u->achievements.insertEnd({title, description});
        history.push({ActionType::AddAchievement, id, "", "", ""});
    }

    void updateProfile(int id, const std::string& name, const std::string& profession,
                       const std::string& experience) {
        User* u = get(id);
        if (!u) {
            throw std::runtime_error("User ID not found.");
        }
        history.push({ActionType::UpdateProfile, id, u->name, u->profession, u->experience});
        u->name = name;
        u->profession = profession;
        u->experience = experience;
        rebuildSearchIndex();
    }

    void connect(int a, int b) {
        User* first = get(a);
        User* second = get(b);
        if (!first || !second || a == b) {
            throw std::runtime_error("Use two different valid user IDs.");
        }
        for (int id : first->connections.toVector()) {
            if (id == b) {
                throw std::runtime_error("These users are already connected.");
            }
        }
        first->connections.insertEnd(b);
        second->connections.insertEnd(a);
        notifications.enqueue({b, first->name + " connected with you."});
    }

    std::vector<User*> sortedByName() {
        BST<User*, UserNameLess> tree;
        for (User* u : users.values()) {
            tree.insert(u);
        }
        return tree.inorderTraversal();
    }

    std::vector<User*> sortedByProfession() {
        BST<User*, UserProfessionLess> tree;
        for (User* u : users.values()) {
            tree.insert(u);
        }
        return tree.inorderTraversal();
    }

    std::vector<User*> search(const std::string& prefix) {
        std::vector<User*> result;
        for (int id : names.searchByPrefix(prefix)) {
            if (User* u = get(id)) {
                result.push_back(u);
            }
        }
        std::sort(result.begin(), result.end(), UserNameLess{});
        return result;
    }

    std::vector<User*> mutualConnections(int a, int b) {
        User* first = get(a);
        User* second = get(b);
        if (!first || !second || a == b) {
            throw std::runtime_error("Use two different valid user IDs.");
        }

        std::vector<int> secondIds = second->connections.toVector();
        std::vector<User*> result;
        for (int id : first->connections.toVector()) {
            bool shared =
                std::find(secondIds.begin(), secondIds.end(), id) != secondIds.end();
            if (shared) {
                if (User* u = get(id)) {
                    result.push_back(u);
                }
            }
        }
        std::sort(result.begin(), result.end(), UserNameLess{});
        return result;
    }

    std::vector<User*> shortestPath(int start, int goal) {
        if (!get(start) || !get(goal)) {
            throw std::runtime_error("User ID not found.");
        }

        Queue<int> frontier;
        HashTable<int, int> parent;
        frontier.enqueue(start);
        parent.insert(start, -1);

        while (!frontier.isEmpty()) {
            int id = frontier.dequeue();
            if (id == goal) {
                break;
            }
            for (int neighbor : get(id)->connections.toVector()) {
                if (!parent.contains(neighbor)) {
                    parent.insert(neighbor, id);
                    frontier.enqueue(neighbor);
                }
            }
        }

        if (!parent.contains(goal)) {
            return {};
        }

        std::vector<User*> path;
        for (int current = goal; current != -1; current = parent.get(current)) {
            path.push_back(get(current));
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    void display(const User* u) const {
        std::cout << "\n[" << u->id << "] " << u->name << " - " << u->profession << "\n";
        std::cout << "Experience: " << u->experience << "\n";

        std::cout << "Skills: ";
        for (const Skill& s : u->skills.toVector()) {
            std::cout << s.name << " (" << s.level << ") ";
        }

        std::cout << "\nAchievements: ";
        for (const Achievement& a : u->achievements.toVector()) {
            std::cout << a.title << "; ";
        }

        std::cout << "\nConnections: " << u->connections.size() << "\n";
    }

    void listByName() {
        for (User* u : sortedByName()) {
            display(u);
        }
    }

    void listByProfession() {
        for (User* u : sortedByProfession()) {
            display(u);
        }
    }

    void processNotification() {
        if (notifications.isEmpty()) {
            std::cout << "No pending notifications.\n";
            return;
        }
        Notification n = notifications.dequeue();
        User* u = get(n.userId);
        std::cout << "To " << (u ? u->name : "deleted user") << ": " << n.message << "\n";
    }

    void undo() {
        if (history.isEmpty()) {
            std::cout << "No undoable action.\n";
            return;
        }

        HistoryEntry action = history.pop();
        User* user = get(action.userId);
        if (!user) {
            throw std::runtime_error("The affected user no longer exists.");
        }

        switch (action.type) {
            case ActionType::AddSkill:
                user->skills.removeAt(user->skills.size() - 1);
                std::cout << "Undid the latest skill addition.\n";
                break;
            case ActionType::AddAchievement:
                user->achievements.removeAt(user->achievements.size() - 1);
                std::cout << "Undid the latest achievement addition.\n";
                break;
            case ActionType::UpdateProfile:
                user->name = action.oldName;
                user->profession = action.oldProfession;
                user->experience = action.oldExperience;
                rebuildSearchIndex();
                std::cout << "Restored the previous profile details.\n";
                break;
        }
    }
};

static std::string ask(const std::string& label) {
    std::cout << label;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

static int askId(const std::string& label) {
    while (true) {
        std::string text = ask(label);
        try {
            size_t parsed = 0;
            int id = std::stoi(text, &parsed);
            if (parsed == text.size()) {
                return id;
            }
        } catch (...) {
        }
        std::cout << "Enter a numeric ID.\n";
    }
}

static void printMenu() {
    std::cout << "\n=== NetWorQ | Professional Networking ===\n"
              << "1. Create profile\n"
              << "2. List profiles alphabetically\n"
              << "3. List profiles by profession\n"
              << "4. Search profiles\n"
              << "5. View profile\n"
              << "6. Update profile\n"
              << "7. Add skill\n"
              << "8. Add achievement\n"
              << "9. Connect users\n"
              << "10. Mutual connections\n"
              << "11. Find shortest connection path\n"
              << "12. Process next notification\n"
              << "13. Undo latest action\n"
              << "0. Exit\n"
              << "Choice: ";
}

static void handleChoice(Network& app, const std::string& choice) {
    if (choice == "1") {
        std::string name = ask("Name: ");
        std::string profession = ask("Profession: ");
        std::string experience = ask("Experience: ");
        User* u = app.addUser(name, profession, experience);
        std::cout << "Profile created. Your ID is " << u->id << ".\n";
    } else if (choice == "2") {
        app.listByName();
    } else if (choice == "3") {
        app.listByProfession();
    } else if (choice == "4") {
        std::vector<User*> found = app.search(ask("Name prefix: "));
        if (found.empty()) {
            std::cout << "No matches.\n";
        }
        for (User* u : found) {
            app.display(u);
        }
    } else if (choice == "5") {
        if (User* u = app.get(askId("User ID: "))) {
            app.display(u);
        } else {
            std::cout << "User not found.\n";
        }
    } else if (choice == "6") {
        int id = askId("User ID: ");
        std::string name = ask("New name: ");
        std::string profession = ask("New profession: ");
        std::string experience = ask("New experience: ");
        app.updateProfile(id, name, profession, experience);
        std::cout << "Profile updated.\n";
    } else if (choice == "7") {
        int id = askId("User ID: ");
        std::string skill = ask("Skill: ");
        std::string level = ask("Level: ");
        app.addSkill(id, skill, level);
    } else if (choice == "8") {
        int id = askId("User ID: ");
        std::string title = ask("Title: ");
        std::string description = ask("Description: ");
        app.addAchievement(id, title, description);
    } else if (choice == "9") {
        int first = askId("First user ID: ");
        int second = askId("Second user ID: ");
        app.connect(first, second);
        std::cout << "Connection created.\n";
    } else if (choice == "10") {
        int first = askId("First user ID: ");
        int second = askId("Second user ID: ");
        std::vector<User*> mutual = app.mutualConnections(first, second);
        if (mutual.empty()) {
            std::cout << "No mutual connections.\n";
        } else {
            std::cout << "Mutual connections: ";
            for (size_t i = 0; i < mutual.size(); ++i) {
                std::cout << (i ? ", " : "") << mutual[i]->name;
            }
            std::cout << "\n";
        }
    } else if (choice == "11") {
        int start = askId("Start user ID: ");
        int target = askId("Target user ID: ");
        std::vector<User*> path = app.shortestPath(start, target);
        if (path.empty()) {
            std::cout << "No connection path exists.\n";
        } else {
            std::cout << "Path: ";
            for (size_t i = 0; i < path.size(); ++i) {
                std::cout << (i ? " -> " : "") << path[i]->name;
            }
            std::cout << "\n";
        }
    } else if (choice == "12") {
        app.processNotification();
    } else if (choice == "13") {
        app.undo();
    } else {
        std::cout << "Unknown choice.\n";
    }
}

int main() {
    Network app;
    app.addUser("Suhan", "Software Engineer", "3 years of experience");
    app.addUser("Aleen", "Data Analyst", "2 years of experience");
    app.addUser("Aditi", "Web Developer", "Fresher");
    std::cout << "Welcome to NetWorQ. Three sample profiles are ready to explore.\n";

    while (true) {
        printMenu();
        std::string choice;
        if (!std::getline(std::cin, choice) || choice == "0") {
            break;
        }
        try {
            handleChoice(app, choice);
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    std::cout << "Goodbye.\n";
}
