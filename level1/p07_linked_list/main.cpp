#include <iostream>
#include <string>
#include <cstddef>
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

    // 按索引插入
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

    // 删除指定位置
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

    // 反转链表：迭代法就地反转
    void reverse() {
        Node<T>* prev = nullptr;
        Node<T>* cur  = head;
        while (cur) {
            Node<T>* next = cur->next;   // 先保存后继，防止断链
            cur->next = prev;            // 当前节点反向指向前驱
            prev = cur;                  // 前驱前进
            cur = next;                  // 当前前进
        }
        head = prev;
    }

    // 查找第一个值为 val 的节点，返回序号（从 0 开始），找不到返回 -1
    int findIndex(const T& val) const {
        int idx = 0;
        for (Node<T>* cur = head; cur; cur = cur->next, ++idx)
            if (cur->data == val) return idx;
        return -1;
    }

    // 从序号 start 之后查找下一个值为 val 的节点，返回序号，找不到返回 -1
    int findNextIndex(const T& val, size_t start) const {
        size_t idx = 0;
        Node<T>* cur = head;
        // 先跳过下标 0..start 的节点
        while (cur && idx < start + 1) { cur = cur->next; ++idx; }
        // 从 start+1 开始查找
        while (cur) {
            if (cur->data == val) return (int)idx;
            cur = cur->next;
            ++idx;
        }
        return -1;
    }

    // 遍历打印（满足 README "遍历链表依次显示各节点 value"）
    void print() const {
        Node<T>* cur = head;
        while (cur) {
            cout << cur->data << " -> ";
            cur = cur->next;
        }
        cout << "nullptr\n";
    }

    // 清空链表
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
    // 1) 创建单链表
    LinkedList<int> list;
    list.insertAtTail(10);
    list.insertAtTail(5);
    list.insertAtTail(20);
    list.insertAtTail(5);
    list.insertAtTail(30);

    // 2) 遍历显示各节点 value
    cout << "原始链表: ";
    list.print();

    // 3) 反序
    list.reverse();
    cout << "反序后:   ";
    list.print();

    // 4) 查找第一个值为 5 的节点序号（找不到返回 -1）
    int first = list.findIndex(5);
    cout << "第一个值为 5 的节点序号: " << first << endl;

    // 5) 查找下一个值为 5 的节点序号
    if (first != -1) {
        int next = list.findNextIndex(5, first);
        cout << "下一个值为 5 的节点序号: " << next << endl;
    }

    // 查找不存在的值，应返回 -1
    cout << "查找 99 的序号: " << list.findIndex(99) << endl;

    // 拷贝构造验证
    LinkedList<int> copy = list;
    cout << "拷贝构造:   ";
    copy.print();

    // 模板通用性验证（string）
    cout << "\nstring 链表: ";
    LinkedList<string> strList;
    strList.insertAtTail("hello");
    strList.insertAtTail("world");
    strList.print();

    return 0;
}
