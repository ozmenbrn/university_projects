:-include('plannerData.pl'). 



append([], Ys, Ys).
append([X|Xs], Ys, [X|Zs]) :-	append(Xs, Ys, Zs).

member(X,[X|_]).
member(X,[_|Ts]) :-
	member(X,Ts).

%Deletes Repeated Elements in a list.
deleteRepeated( [],[]).
deleteRepeated( [H|T],[H|Out]) :-
    not(member(H,T)),
    deleteRepeated(T,Out).
deleteRepeated( [H|T],Out) :-
    member(H,T),
    deleteRepeated(T,Out).

%infood(+Element, ?InitialList,-Meallist). Checks if there is the Element in the ingredient list of the foods. 
infood(_,[],Meallist):-Meallist=[].
infood(H1,[H2|T2], Meallist ):-
 	meal(H2,X,_,_,_),
	member(H1,X),
	Meallist = [H2 | Meallist2],
	infood(H1, T2, Meallist2).
infood(H1,[_|T2], Meallist ):-
	infood(H1, T2, Meallist).

%calorieCheck(+CalorieLimit,?InitialList,-Meallist). Returns the food which are under the calorie limit.
calorieCheck(0,_,_).  
calorieCheck(_,[],Meallist) :- Meallist=[].
calorieCheck(CalLimit,[H2|T2],Meallist) :- 
	meal(H2,_,Calorie,_,_),
	CalLimit < Calorie,
	Meallist= [H2 | Meallist2],
	calorieCheck(CalLimit,T2,Meallist2).

calorieCheck(CalLimit,[_|T1],Meallist):-
	calorieCheck(CalLimit,T1,Meallist).


%findLikeMeals(+Likes, ?InitialList, -Meallist). Checks the foods include one of the likes.
findLikeMeals(_,[],Meallist):-Meallist=[].
findLikeMeals([],_,Meallist):-Meallist=[].
findLikeMeals([H1|T1],L,Meallist):-
	infood(H1, L, Meallist2),
	findLikeMeals(T1, L, Meallist3),
	append(Meallist2, Meallist3, Meallist).	

%findDisLikedMeals(+Likes, ?InitialList, -Meallist). Checks the foods include one of the dislikes, uses infood function.
findDisLikedMeals([],_,Meallist):-Meallist=[].
findDisLikedMeals([H1|T1],L,Meallist):-
	infood(H1, L, Meallist2),
	findDisLikedMeals(T1, L, Meallist3),
	append(Meallist2, Meallist3, Meallist).	


%findAllergyMeals(+AllergyList, ?InitialList, -MealList). Uses finddisliked function. 
findAllergyMeals([],_,Meallist):-Meallist=[].
findAllergyMeals([H1|T1],L,Meallist):-
	foodGroup(H1,[H2|T2]),
	findDisLikedMeals([H2|T2], L, Meallist2),
	findAllergyMeals(T1,L,Meallist3),
	append(Meallist2,Meallist3,Meallist).

%findNotEatingTypeMeals(+EatingTypeList, ?InitialList, -MealList). Uses findAllergyMeals and calorieCheck functions.
findNotEatingTypeMeals([],_,Meallist):-Meallist=[].
findNotEatingTypeMeals([H1|T1],L,Meallist):-
	cannotEatGroup(H1,CannotEatFoodGroupList,CalorieLimit),
	findAllergyMeals(CannotEatFoodGroupList, L, Meallist2),
	calorieCheck(CalorieLimit,L,Meallist3),
	append(Meallist3,Meallist2,Meallist4),
	findNotEatingTypeMeals(T1,L,Meallist5),
	append(Meallist5,Meallist4,Meallist).

%findMealsForTime(+TimeInHand, ?InitialList, -MealList). Returns Meals which are under the time limit.
findMealsForTime(_,[],Meallist):-Meallist=[].
findMealsForTime(PrepTimeLimit,[H1|T1],Meallist):-
	meal(H1,_,_,PreTime,_),
	PreTime =< PrepTimeLimit,
	Meallist = [H1 | Meallist2],
	findMealsForTime(PrepTimeLimit,T1,Meallist2).

findMealsForTime(PrepTimeLimit,[_|T1],Meallist):-
	findMealsForTime(PrepTimeLimit,T1,Meallist).

%findMealsForMoney(+MoneyInHand, ?InitialList, -MealList)
findMealsForMoney(_,[],MealList):-MealList=[].
findMealsForMoney(MoneyInHand,[H1|T1],MealList):-
	meal(H1,_,_,_,Price),
	Price =< MoneyInHand,
	MealList = [H1|MealList2],
	findMealsForMoney(MoneyInHand, T1 , MealList2).

findMealsForMoney(MoneyInHand, [_|T1],MealList):-
	findMealsForMoney(MoneyInHand, T1 , MealList).


%subtract(+Set, +Delete, -Result) 
subtract([], _, []) :- !.
subtract([E|T], D, R) :-
    memberchk(E, D), !,
    subtract(T, D, R).
subtract([H|T], D, [H|R]) :-
    subtract(T, D, R).


%orderLikedList(+LikeMeals, ?InitialList, -MealList) First subtract then adds LikeMeals. Prevents duplication and puts likes in the head of list 
orderLikedList(_,[],MealList):-MealList=[].
orderLikedList(LikeMeals,InitialList,MealList):-
	subtract(InitialList,LikeMeals,Result),
	append(LikeMeals,Result,MealList).		
		

%fullList(FullList) Returns all food in the database.
	fullList(FullList):-
	findall(FoodType ,meal(FoodType,_,_,_,_),X),
	flatten(X,FullList).
	


%listPersonalList(+CustomerName, -PersonalList)
listPersonalList(CustomerName, PersonalList):-
	customer(CustomerName, AllergyList, EatingType, Dislikes, Likes, TimeInHand, MoneyInHand),
	fullList(FullList),
	findMealsForTime(TimeInHand,FullList,TimedMeallist),
		findMealsForMoney(MoneyInHand,TimedMeallist,TimeAndPricedList),
		%TimeAndPricedList is all the foods that the customers can afford and wait.


	findDisLikedMeals(Dislikes,FullList,DislikedMeallist),
	findAllergyMeals(AllergyList,FullList,AllergicMeallist),		
	findNotEatingTypeMeals(EatingType,FullList,NotEatingTypeMeallist),	
	append(AllergicMeallist,NotEatingTypeMeallist,NotFood),
		%Notfood is the list of foods that customer should not eat. 

	findLikeMeals(Likes,TimeAndPricedList,LikedMeallist),	
	
		% 
	subtract(TimeAndPricedList,NotFood,FinalMeallist),
	deleteRepeated(FinalMeallist,List),
	deleteRepeated(LikedMeallist,LikeList),
	orderLikedList(LikeList, List, PerList),
	subtract(PerList,DislikedMeallist,PersonalList).




