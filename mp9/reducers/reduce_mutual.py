# reduce function:
def reduce(left, right):
    # print("left dictionary", left)
    # print("right dictionary", right)
    if len(left) <= 0 and len(right) <= 0:
        return {}
    combined = {}
    for k, v in left.items():
        if k in right:
            intersect = Intersection(v, right[k])
            #print(intersect)
            if len(intersect) > 0:
                combined[k] = intersect
        else:
            combined[k] = v

    for k, v in right.items():
        if k not in left:
            combined[k] = v
    # print(combined)
    return combined

def Intersection(list1, list2):
    return list(set(list1).intersection(list2))
