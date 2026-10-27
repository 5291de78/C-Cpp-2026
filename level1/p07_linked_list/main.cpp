// 题目：单向链表（README 功能点与代码对应关系）
//   1) main 中创建单链表          -> LinkedList<int> list + insertAtTail
//   2) 遍历显示各节点 value        -> print()
//   3) 将所有节点反序             -> reverse()（就地迭代反转）
//   4) 查找第一个值为 5 的节点序号  -> findIndex(5)，找不到返回 -1
//   5) 查找下一个值为 5 的节点序号  -> findNextIndex(5, first)，找不到返回 -1
// 附加：模板化（支持任意类型）、深拷贝（拷贝构造/赋值）、移动语义（移动构造/赋值，规则五）

#include <iostream>
#include <string>
#include <cstddef>
#include <utility>
using namespace std;

template <typename T>
struct Node {
    T data;              // 节点数据
    Node* next;          // 指向下一个节点的指针

    Node(const T& val) : data(val), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;       // 头指针
    size_t size;         // 节点数量

public:
    LinkedList() : head(nullptr), size(0) {}

    // 析构函数：释放所有节点
    ~LinkedList() { clear(); }

    // 拷贝构造函数（深拷贝）：避免默认浅拷贝导致的重复释放 / 悬挂指针
    LinkedList(const LinkedList& other) : head(nullptr), size(0) {
        Node<T>** tail = &head;                    // 指向"下一个要连接的 next"
        for (Node<T>* cur = other.head; cur; cur = cur->next) {
            *tail = new Node<T>(cur->data);
            tail = &((*tail)->next);
        }
        size = other.size;
    }

    // 拷贝赋值运算符：自赋值保护 + 深拷贝重建
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node<T>** tail = &head;
            for (Node<T>* cur = other.head; cur; cur = cur->next) {
                *tail = new Node<T>(cur->data);
                tail = &((*tail)->next);
            }
            size = other.size;
        }
        return *this;
    }

    // 移动构造函数：直接接管源对象的资源，标记为 noexcept 以便容器高效使用
    LinkedList(LinkedList&& other) noexcept : head(other.head), size(other.size) {
        other.head = nullptr;                      // 置空源对象，防止其析构时误删
        other.size = 0;
    }

    // 移动赋值运算符：释放自身资源后接管源对象资源
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            size = other.size;
            other.head = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // ---------- 基本操作 ----------

    // 头插法
    void insertAtHead(const T& val) {
        Node<T>* newNode = new Node<T>(val);
        newNode->next = head;
        head = newNode;
        ++size;
    }

    // 尾插法
    void insertAtTail(const T& val) {
        if (!head) { insertAtHead(val); return; }
        Node<T>* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = new Node<T>(val);
        ++size;
    }

    // 按索引插入（0 表示插到头部），索引非法返回 false
    bool insertAtIndex(size_t index, const T& val) {
        if (index > size) return false;
        if (index == 0) { insertAtHead(val); return true; }
        Node<T>* prev = head;
        for (size_t i = 0; i < index - 1; ++i) prev = prev->next;
        Node<T>* newNode = new Node<T>(val);
        newNode->next = prev->next;
        prev->next = newNode;
        ++size;
        return true;
    }

    // 删除指定位置节点，索引非法返回 false
    bool deleteAtIndex(size_t index) {
        if (index >= size) return false;
        if (index == 0) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            --size;
            return true;
        }
        Node<T>* prev = head;
        for (size_t i = 0; i < index - 1; ++i) prev = prev->next;
        Node<T>* temp = prev->next;
        prev->next = temp->next;
        delete temp;
        --size;
        return true;
    }

    // 反转链表：就地迭代法，三指针（prev/cur/next）原地翻转指向，O(n) 时间、O(1) 空间
    void reverse() {
        Node<T>* prev = nullptr;
        Node<T>* cur  = head;
        while (cur) {
            Node<T>* next = cur->next;   // 先保存后继，防止断链
            cur->next = prev;            // 当前节点反向指向前驱
            prev = cur;                  // 前驱前进
            cur = next;                  // 当前前进
        }
        head = prev;                     // 原来的尾节点成为新头
    }

    // ---------- 查找（README 要求 4、5）----------

    // 查找第一个值为 val 的节点，返回序号（从 0 开始），找不到返回 -1
    int findIndex(const T& val) const {
        int idx = 0;
        for (Node<T>* cur = head; cur; cur = cur->next, ++idx)
            if (cur->data == val) return idx;
        return -1;
    }

    // 从序号 start 之后开始查找下一个值为 val 的节点，返回序号，找不到返回 -1。
    // 配合 findIndex 反复调用，即可依次找出链表中所有值为 val 的节点。
    int findNextIndex(const T& val, size_t start) const {
        size_t idx = 0;
        Node<T>* cur = head;
        // 先跳过下标 0..start 的节点
        while (cur && idx < start + 1) { cur = cur->next; ++idx; }
        // 从 start+1 开始查找
        while (cur) {
            if (cur->data == val) return static_cast<int>(idx);
            cur = cur->next;
            ++idx;
        }
        return -1;
    }

    // ---------- 辅助 ----------

    size_t getSize() const { return size; }
    bool empty() const { return size == 0; }

    // 遍历打印各节点 value（README 要求 2）
    void print() const {
        if (!head) {
            cout << "空链表 (nullptr)\n";
            return;
        }
        Node<T>* cur = head;
        while (cur) {
            cout << cur->data << " -> ";
            cur = cur->next;
        }
        cout << "nullptr\n";
    }

    // 清空链表：逐个释放节点内存
    void clear() {
        Node<T>* cur = head;
        while (cur) {
            Node<T>* next = cur->next;
            delete cur;
            cur = next;
        }
        head = nullptr;
        size = 0;
    }
};

int main() {
    // 1) 创建单链表：10 -> 5 -> 20 -> 5 -> 30（包含两个 5，便于演示两次查找）
    LinkedList<int> list;
    list.insertAtTail(10);
    list.insertAtTail(5);
    list.insertAtTail(20);
    list.insertAtTail(5);
    list.insertAtTail(30);

    // 2) 遍历显示各节点 value
    cout << "原始链表（共 " << list.getSize() << " 个节点）: ";
    list.print();

    // 3) 反序
    list.reverse();
    cout << "反序后:              ";
    list.print();

    // 4) + 5) 两次查找：先找第一个 5，再反复找下一个 5，直到返回 -1
    cout << "链表中所有值为 5 的节点序号: ";
    int idx = list.findIndex(5);                 // 第一次查找：第一个 5
    if (idx == -1) {
        cout << "未找到" << endl;
    } else {
        while (idx != -1) {                      // 后续查找：反复找下一个 5
            cout << idx << " ";
            idx = list.findNextIndex(5, static_cast<size_t>(idx));
        }
        cout << "（查找结束，返回 -1）" << endl;
    }

    // 查找不存在的值，应返回 -1
    cout << "查找 99 的序号: " << list.findIndex(99) << endl;

    // 拷贝构造验证：修改副本不影响原链表
    LinkedList<int> copy = list;
    copy.insertAtHead(99);
    cout << "拷贝后追加 99:       ";
    copy.print();
    cout << "原链表不受影响:      ";
    list.print();

    // 模板通用性验证（string 类型）
    cout << "\nstring 链表: ";
    LinkedList<string> strList;
    strList.insertAtTail("hello");
    strList.insertAtTail("world");
    strList.print();

    return 0;
}
