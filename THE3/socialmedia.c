

#include "socialmedia.h"

#include <stdlib.h>

Database db;
Cache userCache;
Cache postCache;

void printUser(User *user) {
  printf("User: %d, Username: %s, Followers: %d, Following: %d, Liked: %d\n",
         user->userId, user->username, user->numOfFollowers,
         user->numOfFollowing, user->liked);
}

void printPost(Post *post) {
  printf("Post: %d, Author: %s, Likes: %d\n", post->postId,
         post->author->username, post->likes);

    
}



void printUserCache(void) 
{
  Node *current=userCache.head;
  User *user;

  printf("UserCache:\n");
  if(!current)
      return;

  while(current)
  {
    if(current->data)
    {
      user =current->data;

      printUser(user);
    }
    current=current->next;
  }
}




void printPostCache(void)
{
  Node *current=postCache.tail;
  Post *post;
  printf("PostCache:\n");
  if(!current)
    return;

  while(current)
  {
    post=current->data;

    printPost(post);
    current=current->prev;

  }

}




Node *createNode(void *data) 
{ 
  Node *newnode=malloc(sizeof(Node));
  newnode->data=malloc(sizeof(data));
  newnode->data=data;
  newnode->prev=NULL;
  newnode->next=NULL;
  return newnode;
}




void appendToCache(Cache *cache, void *data) 
{ 
  Node *current=cache->head;
  Node *newnode;
  newnode=createNode(data);

  if(!current)
  {
    cache->head=newnode;
    cache->tail=newnode;
    cache->size++;
  }
  else
  {
  cache->head->prev=newnode;
  newnode->next=cache->head;
  cache->head=newnode;
  cache->size++;

  }






}






void moveToFront(Cache *cache, Node *node)
{ 


  if(cache->head==node)
    return;
  else if(cache->capacity==1)
    return;
  else if(cache->tail==node)
  {
    cache->head->prev=node;
    node->next=cache->head;
    cache->tail=node->prev;
    node->prev=NULL;
    cache->head=node;
    cache->tail->next=NULL;
  }
  else
  {
    node->prev->next=node->next;
    node->next->prev=node->prev;
    node->next=cache->head;
    node->prev=NULL;
    cache->head->prev=node;
    cache->head=node;
  }






}

void removeLast(Cache *cache) 
{ 
  Node *prev;
  if(cache->capacity==1)
  {
    free(cache->tail);
    cache->tail=NULL;
    cache->head=NULL;
  }
  else
  {
    prev=cache->tail->prev;
    free(cache->tail);
    cache->tail=prev;
    prev->next=NULL;
    cache->size--;
  }    
    
    
    
    
    
}







User *fetchUser(int userId)
{ 
  Node *current=userCache.head;
  User *userptr;

  int control=0;



  if(!current)
  {
    userptr=&(db.users[userId]);
    appendToCache(&userCache, userptr);
    return userptr;
  }

  while(current && control==0)
  {
    userptr=current->data;
    if(userptr->userId!=userId)
    {
      current=current->next;
    }
    else if(userptr->userId==userId)
    {
      control=25;
    }
  }

  if(control==25)
  {
    userptr=current->data;
    moveToFront(&userCache, current);
    return userptr;
  }
  else
  {
    if(userCache.size==userCache.capacity)
    {
      removeLast(&userCache);
      userptr=&(db.users[userId]);
      appendToCache(&userCache, userptr);

      return userptr;
    }

  else
  {
    userptr=&(db.users[userId]);
    appendToCache(&userCache, userptr);
    return userptr;
  }


  }

}









 




Post *fetchPost(int postId)
{ 
  Node *current=postCache.head;
  Post *postptr;
  int control=0;

  if(!current)
  {
    postptr=&(db.posts[postId]);
    appendToCache(&postCache,postptr);
    return postptr;
  }

  while(current && control==0)
  {

    postptr=current->data;
    if(postptr->postId!=postId)
    {
      current=current->next;
    }
    else if(postptr->postId==postId)
    {
      control=25;
    }

  }


  if(control==25)
  {

    postptr=current->data;
    moveToFront(&postCache, current);
    return postptr;
  }
  else
  {
    if(postCache.size==postCache.capacity)
    {


      removeLast(&postCache);

      postptr=&(db.posts[postId]);

      appendToCache(&postCache, postptr);

      return postptr;
    }

    else
    {
      postptr=&(db.posts[postId]);
      appendToCache(&postCache, postptr);
      return postptr;
    }
  }    
}






void followUser(int followerId, int followedId)
{
  User *follower, *followed;
  Node *followerptr;
  Node *current;



  follower=fetchUser(followerId);
  followed= fetchUser(followedId);



  followerptr=createNode(follower);
  current=followed->followers_head;
  if(!current)
  {
    followed->followers_head=followerptr;
    followed->followers_tail=followerptr;
  }
  else
  {
    followerptr->prev=followed->followers_tail;
    followed->followers_tail->next=followerptr;
    followed->followers_tail=followerptr;
  }
  follower->numOfFollowing++;
  followed->numOfFollowers++;
}



void likePost(int userId, int postId)
{ 
  User *person;
  Post *post;



  person=fetchUser(userId);
  post=fetchPost(postId);



  person->liked++;
  post->likes++;
}






void printFollowersInReverse(void) 
{
  Node *ptruser=userCache.tail;
  User *followed=ptruser->data;
  Node *ptrfollower=followed->followers_tail;
  User *followprint=ptrfollower->data;
  printf("Followers of all users in UserCache in reverse:\n");
  if(!ptruser)

    return;

  else
  {
    if(!ptrfollower)
      return;

    while(ptruser)
    {
      followed=ptruser->data;
      ptrfollower=followed->followers_tail;
      printf("Followers of user with id %d in reverse:\n", followed->userId);
      while(ptrfollower)
      {
        followprint=ptrfollower->data;

        printf("[%d] ",followprint->userId);
        ptrfollower=ptrfollower->prev;
      }
      printf("\n");
      ptruser=ptruser->prev;
    }

  }
}


void freeLinkedList(Node *head) 
{
  Node *node=head;
  Node *nextnode;
  while(node)
  {
    nextnode=node->next;
    free(node);
    node=nextnode;

  }







}

