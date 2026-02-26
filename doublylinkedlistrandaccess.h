#ifndef DOUBLYLINKEDLISTRANDACCESS_H
#define DOUBLYLINKEDLISTRANDACCESS_H

#include <string>
#include <vector>
#include <cstdint>

class DoublyLinkedListSerializer;

class DoublyLinkedListRandAccess {
    friend class DoublyLinkedListSerializer;

private:
    struct ListNode {
        ListNode* prev = nullptr;
        ListNode* next = nullptr;
        ListNode* rand = nullptr;
        std::string data;
    };

    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    size_t count = 0;

    void Clear() noexcept;

    static void DestroyNodes(std::vector<ListNode*>& nodes) noexcept;

    static std::vector<ListNode*> BuildChain(std::vector<std::pair<std::string, int32_t>>& entries);

    void CommitChain(std::vector<ListNode*>& nodes) noexcept;

public:
    DoublyLinkedListRandAccess() = default;
    DoublyLinkedListRandAccess(const DoublyLinkedListRandAccess& other);
    DoublyLinkedListRandAccess(DoublyLinkedListRandAccess&& other) noexcept;
    DoublyLinkedListRandAccess& operator=(const DoublyLinkedListRandAccess& other);
    DoublyLinkedListRandAccess& operator=(DoublyLinkedListRandAccess&& other) noexcept;
    ~DoublyLinkedListRandAccess();
};


#endif // DOUBLYLINKEDLISTRANDACCESS_H
