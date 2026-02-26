#include "doublylinkedlistrandaccess.h"

#include <unordered_map>

void DoublyLinkedListRandAccess::Clear() noexcept {
    ListNode* cur = head;
    while (cur) {
        ListNode* next = cur->next;
        delete cur;
        cur = next;
    }
    head = nullptr;
    tail = nullptr;
    count = 0;
}

void DoublyLinkedListRandAccess::DestroyNodes(std::vector<ListNode*>& nodes) noexcept {
    for (ListNode* node : nodes)
        delete node;
    nodes.clear();
}

std::vector<DoublyLinkedListRandAccess::ListNode*> DoublyLinkedListRandAccess::BuildChain(std::vector<std::pair<std::string, int32_t>>& entries)
{
    std::vector<ListNode*> nodes;
    nodes.reserve(entries.size());

    for (auto& [data, _] : entries) {
        ListNode* node = new ListNode();
        node->data = std::move(data);
        nodes.push_back(node);
    }

    for (size_t i = 0; i < nodes.size(); ++i) {
        nodes[i]->prev = (i > 0) ? nodes[i - 1] : nullptr;
        nodes[i]->next = (i + 1 < nodes.size()) ? nodes[i + 1] : nullptr;
        int32_t ri = entries[i].second;
        nodes[i]->rand = (ri >= 0 && static_cast<size_t>(ri) < nodes.size())
                             ? nodes[ri] : nullptr;
    }

    return nodes;
}

void DoublyLinkedListRandAccess::CommitChain(std::vector<ListNode*>& nodes) noexcept {
    Clear();
    head = nodes.empty() ? nullptr : nodes.front();
    tail = nodes.empty() ? nullptr : nodes.back();
    count = nodes.size();
}

DoublyLinkedListRandAccess::DoublyLinkedListRandAccess(const DoublyLinkedListRandAccess& other) {
    if (!other.head)
        return;

    std::vector<ListNode*> otherNodes;
    otherNodes.reserve(other.count);
    for (ListNode* cur = other.head; cur; cur = cur->next)
        otherNodes.push_back(cur);

    std::unordered_map<ListNode*, int32_t> indexMap;
    indexMap.reserve(otherNodes.size());
    for (size_t i = 0; i < otherNodes.size(); ++i)
        indexMap[otherNodes[i]] = static_cast<int32_t>(i);

    std::vector<ListNode*> newNodes;
    newNodes.reserve(other.count);

    try {
        for (ListNode* cur : otherNodes) {
            ListNode* node = new ListNode();
            node->data = cur->data;
            newNodes.push_back(node);
        }
    } catch (...) {
        DestroyNodes(newNodes);
        throw;
    }

    for (size_t i = 0; i < newNodes.size(); ++i) {
        newNodes[i]->prev = (i > 0) ? newNodes[i - 1] : nullptr;
        newNodes[i]->next = (i + 1 < newNodes.size()) ? newNodes[i + 1] : nullptr;
        ListNode* otherRand = otherNodes[i]->rand;
        if (otherRand) {
            int32_t ri = indexMap.at(otherRand);
            newNodes[i]->rand = newNodes[ri];
        }
    }

    head = newNodes.front();
    tail = newNodes.back();
    count = newNodes.size();
}

DoublyLinkedListRandAccess::DoublyLinkedListRandAccess(DoublyLinkedListRandAccess&& other) noexcept : head(other.head), tail(other.tail), count(other.count)
{
    other.head = nullptr;
    other.tail = nullptr;
    other.count = 0;
}

DoublyLinkedListRandAccess& DoublyLinkedListRandAccess::operator=(const DoublyLinkedListRandAccess& other)
{
    if (this != &other) {
        DoublyLinkedListRandAccess tmp(other);
        std::swap(head, tmp.head);
        std::swap(tail, tmp.tail);
        std::swap(count, tmp.count);
    }

    return *this;
}

DoublyLinkedListRandAccess& DoublyLinkedListRandAccess::operator=(DoublyLinkedListRandAccess&& other) noexcept
{
    if (this != &other) {
        Clear();
        head = other.head;
        tail = other.tail;
        count = other.count;
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }
    return *this;
}

DoublyLinkedListRandAccess::~DoublyLinkedListRandAccess() {
    Clear();
}
