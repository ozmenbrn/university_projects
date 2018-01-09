:-dynamic student/2.
:-dynamic available_slots/1.
:-dynamic room_capacity/2.

%clear_knowledge_base.
clear_knowledge_base :-
    findall(X, student(X,_),  StudentList), length(StudentList, StudentNum),
    retractall(student(_,_)), write('Number of students are removed from "student/2":  '), write(StudentNum), nl,

    findall(X, available_slots(X), SlotList), length(SlotList, SlotNum),
    retractall(available_slots(_)), write('Number of available slots  are removed from "available_slots/1":  '), write(SlotNum), nl,

    findall(X, room_capacity(X,_), RoomList), length(RoomList, RoomNum),
    retractall(room_capacity(_,_)), write('Number of rooms are removed from "room_capacity/2":  '), write(RoomNum).


%all_students( -StudentList ).
all_students(StudentList) :-
    findall(X, student(X, _), StudentList).


%all_courses( -CourseList ).
all_courses(CourseList) :-
    findall(X, student(_,X), List),
    flatten(List, List2),
    list_to_set(List2, CourseList).


%counts how many students take the course.
count([],_,0).
count([H|T], H, Result) :- count(T, H, X), Result is X+1.
count([Head|T], H, Result) :- Head\=H, count(T,H,Result).

%student_count( +CourseID, -StudentCount).
student_count(CourseID, StudentCount):-
    findall(X, student(_,X), List),
    flatten(List, List2),
    count(List2, CourseID, StudentCount),!.

% Method for Counting how many students take both lesson with
% member function
count2([],_,_,0).
count2([H|T], ID1, ID2, Result) :- member(ID1, H), member(ID2, H) ,count2(T, ID1, ID2, X), Result is X+1.
count2([H|T], ID1, ID2, Result) :- not(member(ID1, H)) , count2(T, ID1, ID2, Result).
count2([H|T], ID1, ID2, Result) :- not(member(ID2, H)) , count2(T, ID1, ID2, Result).

%common_students( +CourseID1, +CourseID2, -StudentCount).
common_students(CourseID1, CourseID2, StudentCount) :-
    findall(X, student(_,X), List),
    count2(List, CourseID1, CourseID2, StudentCount),!.



room_list(RoomList) :- findall(X, room_capacity(X,_), List), flatten(List, RoomList).

%Check the given final plan fits with the given course and slot.
check_conflict(_,[],_).
check_conflict(Course, [[L1,_,Slot]|T], Slot) :- common_students(Course, L1, StudentCount), StudentCount == 0, check_conflict(Course, T, Slot).
check_conflict(Course, [[_, _ ,L3]| T], Slot) :- L3\=Slot, check_conflict(Course, T, Slot).

%Check 2 same room and slot is not there.
check_slot(_, [], _).
check_slot(Room, [[_, L2, Slot]|T], Slot) :- Room\=L2  ,check_slot(Room, T, Slot).
check_slot(Room, [[_, Room, L3]|T], Slot) :- Slot\=L3 ,check_slot(Room, T, Slot).
check_slot(Room, [[_, L2, L3]|T], Slot) :- Room\=L2 , Slot\=L3 ,check_slot(Room, T, Slot).

%checks the capacity of the Course and Room and return true if it's okey
check_capacity(Course,Room):- student_count(Course,StudentNum),room_capacity(Room,Capacity), StudentNum =<Capacity.

% This method should get Courses recursively and display different Final
% Plans in each time. But, there is a problem, the only different
% element in my List is the first element which actually causes only
% RoomNumber*SlotNumber variations. Sometime even the room also be
% same and in that case, variatons goes down to smaller than
% 1*slotnumbers, Whole other elements stay same because of sending
% spesific courseList to the recursion so, couldn't recursive perfectly.
% Whole changes when we type '.' and run again instead of typing ';'.
final_slots([], _,_ ,FinalPlan, FinalPlan).
final_slots(CourseList , RoomList, SlotList, FTemp,FinalPlan) :- %FTemp for holding a temporary List to check and Add if okey.
    random_member(Room,RoomList), random_select(Course, CourseList, RestCourseList), random_member(Slot, SlotList),
    check_capacity(Course, Room), check_conflict(Course, FTemp, Slot), check_slot(Room, FTemp, Slot),
    append([[Course, Room, Slot]], FTemp, TempList), %Temp List is for adding errorless element with at last FinalPlan.
    final_slots(RestCourseList, RoomList, SlotList, TempList, FinalPlan).
final_slots(CourseList, [HeadRoom|TailRoom], [HeadSlot|TailSlot],FTemp, FinalPlan) :-
    random_select(Course, CourseList, RestCourseList), append([Course], RestCourseList, NewCourseList),
    append(TailRoom, [HeadRoom], RoomList), append(TailSlot, [HeadSlot], Slot),
    final_slots(NewCourseList, RoomList, Slot, FTemp, FinalPlan).

%FinalPlan( -FinalPlan).
final_plan(FinalPlan) :-
    findall(X, available_slots(X), Slots), flatten(Slots, AvailableSlots),
    all_courses(CourseList), room_list(RoomList),
    final_slots(CourseList, RoomList, AvailableSlots ,[],FinalPlan).




%lookup_slot(FinalPlan, Head of Element, Count, Count)
lookup_slot([],_,Count, Count).  %Base Case.
lookup_slot([[Lesson1,_,Slot]|T], [Lesson2,M,Slot], TempCount, Sum) :- common_students(Lesson1, Lesson2, StudentCount), StudentCount \=0,
    lookup_slot(T, [Lesson2,M,Slot], Temp, Sum), Temp is TempCount + StudentCount. %Count If There are Students  who takes both classes at same time
lookup_slot([[Lesson1,_,Slot]|T], [Lesson2,M,Slot],TempCount, Sum) :- common_students(Lesson1, Lesson2, StudentCount), StudentCount == 0,
    lookup_slot(T, [Lesson2,M,Slot], TempCount, Sum).  %If there are no  common students in the given same Slots.
lookup_slot([[_,_,Slot1]|T], [Lesson2,M,Slot2], TempCount, Sum) :- Slot1\=Slot2,
    lookup_slot(T, [Lesson2,M,Slot2],TempCount, Sum). %Otherwise continue with the tail Plan.

%conflict_slot(FinalPlan, Head of FinalPlan, ErrorsCount)
conflict_slot([],_,ErrorCount, ErrorCount):-!.
conflict_slot([HeadPlan|TailPlan], [HeadPlan|_] , TempCount, ErrorCount) :- lookup_slot(TailPlan, HeadPlan, TempCount, Sum),
    conflict_slot(TailPlan, TailPlan, Sum ,ErrorCount).


%conflict_capacity(FinalPlan, Count).
conflict_capacity([], 0). %base Cases.
conflict_capacity([], _). %base Cases.
conflict_capacity([[L1,L2,_]|T], ErrorCount) :- student_count(L1,StudentCount),room_capacity(L2,Capacity), StudentCount =< Capacity,
    conflict_capacity(T, ErrorCount).    %If Capacity is larger than it means no problem. Continue with the tail of the list.
conflict_capacity([[L1,L2,_]|T], ErrorCount) :- student_count(L1,StudentCount),room_capacity(L2,Capacity), StudentCount > Capacity,
    conflict_capacity(T, TempCount), ErrorCount is TempCount + (StudentCount - Capacity). % If not, Error goes up by ErrorCount.


%lookup_same(FinalPlan, HeadOfFinalPLan, ErrorCount, ErrorCount).
lookup_same([],_,Count, Count).   %return if the final plan list is finished.
lookup_same([[_,Room,Slot]|T], [Lesson,Room,Slot],TempCount, ErrorCount) :- lookup_same(T, [Lesson,Room,Slot], Temp, ErrorCount),
    Temp is TempCount+1.         %Check if the room's and Slot's are same, if same add  ErrorCount by 1.
lookup_same([[_,Room1,Slot1]|T], [Lesson,Room2,Slot2], TempCount ,ErrorCount) :- (Room1 \= Room2 ; Slot1\=Slot2),
    lookup_same(T, [Lesson,Room2,Slot2], TempCount, ErrorCount).  %If not take head of element and continue with tail.

%same_slot_room(FinalPlan, HeadFinal, Error, Error)
same_slot_room([],_, ErrorCount, ErrorCount).  %return if finished.
same_slot_room([HeadPlan|TailPlan], [HeadPlan|_], TempCount,ErrorCount) :- lookup_same(TailPlan, HeadPlan, TempCount, Sum),
    same_slot_room(TailPlan, TailPlan, Sum, ErrorCount).   %Send Tail Plan and HeadPlan to lookup_same method.


%conflict_for_plan(+FinalPlan, -ErrorCount).
conflict_for_plan(FinalPlan, ErrorCount) :-
    conflict_capacity(FinalPlan, Count1),     %check capacity and add it to Count1
    conflict_slot(FinalPlan, FinalPlan, Count1, Count2),  %Check common students and adding Count1 to Count2
    same_slot_room(FinalPlan, FinalPlan, Count2, ErrorCount). %Check if there is same slot and same room and Return ERROR COUNT.


