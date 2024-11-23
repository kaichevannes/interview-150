#include "01_linked_list_cycle.h"

bool LinkedListCycle::hasCycle(ListNode *head) {
  ListNode* slow = head;
  ListNode* fast = head;

  while (fast != NULL && fast->next != NULL) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast) return true;
  }
  return false;
}