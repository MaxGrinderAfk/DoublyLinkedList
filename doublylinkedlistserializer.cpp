#include "doublylinkedlistserializer.h"
#include "listutils.h"
#include <unordered_map>
#include <stdexcept>
#include <cstdint>

static constexpr uint32_t MAX_NODES    = 1000000;
static constexpr uint32_t MAX_DATA_LEN = 1000;

template <typename T>
void DoublyLinkedListSerializer::ReadBinary(std::ifstream& fin, T& value) {
    if (!fin.read(reinterpret_cast<char*>(&value), sizeof(T)))
        throw std::runtime_error("Unexpected end of binary file or read error");
}

void DoublyLinkedListSerializer::LoadFromText(DoublyLinkedListRandAccess& list, const std::string& filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
        throw std::runtime_error("Cannot open input file: " + filename);

    std::vector<std::pair<std::string, int32_t>> entries;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;

        if (entries.size() >= MAX_NODES)
            throw std::runtime_error("Node count exceeds maximum of " + std::to_string(MAX_NODES));

        size_t sep = line.rfind(';');
        if (sep == std::string::npos)
            throw std::runtime_error("Invalid format: " + line);

        std::string data = ListUtils::UnescapeData(line.substr(0, sep));

        if (data.size() > MAX_DATA_LEN)
            throw std::runtime_error("Data length " + std::to_string(data.size()) + " exceeds maximum of " + std::to_string(MAX_DATA_LEN));

        int32_t randIdx = std::stoi(line.substr(sep + 1));
        entries.emplace_back(std::move(data), randIdx);
    }

    if (fin.bad())
        throw std::runtime_error("Read error while reading: " + filename);

    std::vector<DoublyLinkedListRandAccess::ListNode*> nodes;
    try {
        nodes = DoublyLinkedListRandAccess::BuildChain(entries);
    } catch (...) {
        DoublyLinkedListRandAccess::DestroyNodes(nodes);
        throw;
    }

    list.CommitChain(nodes);
}

void DoublyLinkedListSerializer::SaveToText(const DoublyLinkedListRandAccess& list, const std::string& filename)
{
    std::vector<DoublyLinkedListRandAccess::ListNode*> nodes;
    nodes.reserve(list.count);
    for (DoublyLinkedListRandAccess::ListNode* cur = list.head; cur; cur = cur->next)
        nodes.push_back(cur);

    std::unordered_map<DoublyLinkedListRandAccess::ListNode*, int32_t> indexMap;
    indexMap.reserve(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i)
        indexMap[nodes[i]] = static_cast<int32_t>(i);

    std::ofstream fout(filename);
    if (!fout.is_open())
        throw std::runtime_error("Cannot open output text file: " + filename);

    for (DoublyLinkedListRandAccess::ListNode* node : nodes) {
        int32_t randIdx = node->rand ? indexMap.at(node->rand) : -1;
        fout << ListUtils::EscapeData(node->data) << ';' << randIdx << '\n';
    }

    if (!fout)
        throw std::runtime_error("Write error while saving text to: " + filename);
}

void DoublyLinkedListSerializer::Serialize(const DoublyLinkedListRandAccess& list, const std::string& filename)
{
    std::ofstream fout(filename, std::ios::binary);
    if (!fout.is_open())
        throw std::runtime_error("Cannot open output file: " + filename);

    std::vector<DoublyLinkedListRandAccess::ListNode*> nodes;
    nodes.reserve(list.count);
    for (DoublyLinkedListRandAccess::ListNode* cur = list.head; cur; cur = cur->next) nodes.push_back(cur);

    std::unordered_map<DoublyLinkedListRandAccess::ListNode*, int32_t> indexMap;
    indexMap.reserve(nodes.size());
    for (size_t i = 0; i < nodes.size(); ++i)
        indexMap[nodes[i]] = static_cast<int32_t>(i);

    uint32_t n = static_cast<uint32_t>(nodes.size());
    fout.write(reinterpret_cast<const char*>(&n), sizeof(n));

    for (DoublyLinkedListRandAccess::ListNode* node : nodes) {
        uint32_t dataLen = static_cast<uint32_t>(node->data.size());
        fout.write(reinterpret_cast<const char*>(&dataLen), sizeof(dataLen));
        fout.write(node->data.data(), dataLen);
        int32_t randIdx = node->rand ? indexMap.at(node->rand) : -1;
        fout.write(reinterpret_cast<const char*>(&randIdx), sizeof(randIdx));
    }

    if (!fout)
        throw std::runtime_error("Write error while serializing to: " + filename);
}

void DoublyLinkedListSerializer::Deserialize(DoublyLinkedListRandAccess& list, const std::string& filename)
{
    std::ifstream fin(filename, std::ios::binary);
    if (!fin.is_open())
        throw std::runtime_error("Cannot open binary file: " + filename);

    uint32_t n = 0;
    ReadBinary(fin, n);

    if (n > MAX_NODES)
        throw std::runtime_error("Node count " + std::to_string(n) + " exceeds maximum of " + std::to_string(MAX_NODES));

    std::vector<DoublyLinkedListRandAccess::ListNode*> nodes;
    nodes.reserve(n);
    std::vector<int32_t> randIndices;
    randIndices.reserve(n);

    try {
        for (uint32_t i = 0; i < n; ++i) {
            uint32_t dataLen = 0;
            ReadBinary(fin, dataLen);

            if (dataLen > MAX_DATA_LEN)
                throw std::runtime_error("Data length " + std::to_string(dataLen) + " exceeds maximum of " + std::to_string(MAX_DATA_LEN));

            std::string data(dataLen, '\0');
            if (!fin.read(data.data(), dataLen))
                throw std::runtime_error("Unexpected end of file reading node data");

            int32_t randIdx = -1;
            ReadBinary(fin, randIdx);

            DoublyLinkedListRandAccess::ListNode* node = new DoublyLinkedListRandAccess::ListNode();
            node->data = std::move(data);
            nodes.push_back(node);
            randIndices.push_back(randIdx);
        }
    } catch (...) {
        DoublyLinkedListRandAccess::DestroyNodes(nodes);
        throw;
    }

    for (size_t i = 0; i < nodes.size(); ++i) {
        nodes[i]->prev = (i > 0) ? nodes[i - 1] : nullptr;
        nodes[i]->next = (i + 1 < nodes.size()) ? nodes[i + 1] : nullptr;
        int32_t ri = randIndices[i];
        nodes[i]->rand = (ri >= 0 && static_cast<size_t>(ri) < nodes.size()) ? nodes[ri] : nullptr;
    }

    list.CommitChain(nodes);
}
