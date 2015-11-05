```python
1 Load cascade classifiers trained
2 Open Web Cam stream
3 while true:
4   read frame from stream
5   convert frame to grey
6   if eye template has been built:
7     track eyes using the eye template
8   else:
9     detect eyes with Viola Johns method and build eye template
11  save eye position
12  draw grid
13  draw rectangle for enclosing eye
14  generate direction command using eye position
15  send command to embedded system
```

---
```python
class Feature
    p0, p1, p2, p3
    weight

class Node
    feature
    threshold
    left
    right

class Classifier
    count
    node
    alpha

class StageClassifier
    count
    threshold
    classifier
    two_rects
    next
    child
    parent

class ClassifierCascade
    count
    isStumpBased
    has_tilted_features
    is_tree
    inv_window_area
    sum, sqsum, tilted
    stage_classifier
    pq0, pq1, pq2, pq3
    p0, p1, p2, p3
    ipp_stages

function create_cascade( stage_count )
    cascade = 0
    block_size = sizeof(cascade) + stage_count * sizeof(cascade.stage_classifier)
    cascade = allococate( block_size )
    clear(cascade)
    cascade.stage_classifier = cascade + 1
    cascade.count = stage_count
    return cascade

function release_cascade( _cascade )
    if( _cascade )
        cascade = _cascade
        if( condition met and cascade.ipp_stages )
            for i = 0 to cascade.count - 1
                if( cascade.ipp_stages[i] )
                    haarClassifierFree_32f( cascade.ipp_stages[i] )
                    free(cascade.ipp_stages[i])
        free( cascade.ipp_stages )
        free( _cascade )

function create_classifier_cascade( cascade )
    features = 0
    weights = 0, thresholds = 0, val1 = 0, val2 = 0
    ipp_counts = 0
    out = 0
    i, j, k, l
    datasize
    total_classifiers = 0
    total_nodes = 0
    haar_classifier_ptr
    haar_node_ptr
    orig_window_size
    has_tilted_features = 0
    max_count = 0
    orig_window_size = cascade.orig_window_size
    for i = 0 to cascade.count - 1
        stage_classifier = cascade.stage_classifier + i
        max_count = max( max_count, stage_classifier.count )
        total_classifiers += stage_classifier.count
        for j = 0 to stage_classifier.count - 1
            classifier = stage_classifier.classifier + j
            total_nodes += classifier.count
            for l = 0 to classifier.count - 1
                for k = 0 to max_feature - 1
                    if( classifier.haar_feature[l].rect[k].r.width )
                        r = classifier.haar_feature[l].rect[k].r
                        tilted = classifier.haar_feature[l].tilted
                        has_tilted_features |= tilted is not 0;
    datasize = sizeof(hidHaarClassifierCascade) +
               sizeof(hidHaarStageClassifier) * cascade.count +
               sizeof(CvHidHaarClassifier) * total_classifiers +
               sizeof(CvHidHaarTreeNode) * total_nodes +
               total_nodes + total_classifiers
    out = allocate( datasize )
    out.count = cascade.count
    out.stage_classifier = out + 1
    haar_classifier_ptr = out.stage_classifier + cascade.count
    haar_node_ptr = haar_classifier_ptr + total_classifiers
    out.isStumpBased = 1
    out.has_tilted_features = has_tilted_features
    out.is_tree = 0
    for i = 0 to cascade.count - 1
        stage_classifier = cascade.stage_classifier + i
        hid_stage_classifier = out.stage_classifier + i
        hid_stage_classifier.count = stage_classifier.count
        hid_stage_classifier.threshold = stage_classifier.threshold - bias
        hid_stage_classifier.classifier = haar_classifier_ptr
        hid_stage_classifier.two_rects = 1
        haar_classifier_ptr += stage_classifier.count
        out.is_tree |= hid_stage_classifier.next
        for j = 0 to stage_classifier.count - 1
            classifier = stage_classifier.classifier + j
            hid_classifier = hid_stage_classifier.classifier + j
            node_count = classifier.count
            alpha_ptr = haar_node_ptr + node_count
            hid_classifier.count = node_count
            hid_classifier.node = haar_node_ptr
            hid_classifier.alpha = alpha_ptr
            for l = 0 to node_count - 1
                node = hid_classifier.node + l
                feature = classifier.haar_feature + l
                node.threshold = classifier.threshold[l]
                node->left = classifier.left[l]
                node->right = classifier.right[l]
            out.isStumpBased &= node_count == 1
    return out

function set_images_for_classifier_cascade( _cascade, _sum, _sqsum, _tilted_sum, scale )
    sum_stub, sum = _sum
    sqsum_stub, sqsum = _sqsum
    tilted_stub, tilted = _tilted_sum
    cascade
    coi0 = 0, coi1 = 0
    equRect
    weight_scale
    sum = get_image( sum, sum_stub, coi0 )
    sqsum = get_image( sqsum, sqsum_stub, coi1 )
    if( not _cascade.hid_cascade )
        createHidHaarClassifierCascade(_cascade)
    cascade = _cascade.hid_cascade
    _cascade.scale = scale
    _cascade.real_window_size.width = round( _cascade.orig_window_size.width * scale )
    _cascade->real_window_size.height = round( _cascade->orig_window_size.height * scale )
    cascade.sum = sum
    cascade.sqsum = sqsum
    equRect.x = equRect.y = round(scale)
    equRect.width = round((_cascade.orig_window_size.width - 2) * scale)
    equRect.height = round((_cascade->orig_window_size.height-2) * scale)
    weight_scale = 1 / (equRect.width * equRect.height)
    cascade.inv_window_area = weight_scale
    cascade.p0 = sum_elem_ptr(sum, equRect.y, equRect.x)
    cascade.p1 = sum_elem_ptr(sum, equRect.y, equRect.x + equRect.width )
    cascade.p2 = sum_elem_ptr(sum, equRect.y + equRect.height, equRect.x )
    cascade.p3 = sum_elem_ptr(sum, equRect.y + equRect.height, equRect.x + equRect.width )
    cascade.pq0 = sqsum_elem_ptr(*sqsum, equRect.y, equRect.x)
    cascade.pq1 = sqsum_elem_ptr(*sqsum, equRect.y, equRect.x + equRect.width )
    cascade.pq2 = sqsum_elem_ptr(*sqsum, equRect.y + equRect.height, equRect.x )
    cascade.pq3 = sqsum_elem_ptr(*sqsum, equRect.y + equRect.height, equRect.x + equRect.width )
    for i = 0 to _cascade.count
        for j = 0 to cascade.stage_classifier[i].count - 1
            for l = 0 to cascade.stage_classifier[i].classifier[j].count - 1
                feature = _cascade.stage_classifier[i].classifier[j].haar_feature[l]
                classifier = cascade.stage_classifier[i].classifier + j
                hidfeature = cascade.stage_classifier[i].classifier[j].node[l].feature
                sum0 = 0, area0 = 0
                base_w = -1, base_h = -1
                new_base_w = 0, new_base_h = 0
                kx, ky
                flagx = 0, flagy = 0
                x0 = 0, y0 = 0
                for k = 0 to max_feature - 1
                    r[k] = feature.rect[k].r
                    base_w = min( base_w, r[k].width - 1
                    base_w = min( base_w, r[k].x - r[0].x - 1 )
                    base_h = min( base_h, r[k].height-1 )
                    base_h = min( base_h, r[k].y - r[0].y - 1 )
                nr = k
                base_w += 1
                base_h += 1
                kx = r[0].width / base_w
                ky = r[0].height / base_h
                if( kx <= 0 )
                    flagx = 1
                    new_base_w = round( r[0].width * scale ) / kx
                    x0 = round( r[0].x * scale )
                if( ky <= 0 )
                    flagy = 1
                    new_base_h = round( r[0].height * scale ) / ky
                    y0 = round( r[0].y * scale )
                for k = 0 to nr - 1
                    if( flagx )
                        tr.x = (r[k].x - r[0].x) * new_base_w / base_w + x0
                        tr.width = r[k].width * new_base_w / base_w
                    else
                        tr.x = round( r[k].x * scale )
                        tr.width = round( r[k].width * scale )
                    if( flagy )
                        tr.y = (r[k].y - r[0].y) * new_base_h / base_h + y0
                        tr.height = r[k].height * new_base_h / base_h
                    else
                        tr.y = round( r[k].y * scale )
                        tr.height = round( r[k].height * scale )
                    if( not feature.tilted )
                        hidfeature.rect[k].p0 = sum_elem_ptr(sum, tr.y, tr.x)
                        hidfeature.rect[k].p1 = sum_elem_ptr(sum, tr.y, tr.x + tr.width)
                        hidfeature.rect[k].p2 = sum_elem_ptr(sum, tr.y + tr.height, tr.x)
                        hidfeature.rect[k].p3 = sum_elem_ptr(sum, tr.y + tr.height, tr.x + tr.width)
                    else
                        hidfeature.rect[k].p2 = sum_elem_ptr(tilted, tr.y + tr.width, tr.x + tr.width)
                        hidfeature.rect[k].p3 = sum_elem_ptr(tilted, tr.y + tr.width + tr.height, tr.x + tr.width - tr.height)
                        hidfeature.rect[k].p0 = sum_elem_ptr(tilted, tr.y, tr.x)
                        hidfeature.rect[k].p1 = sum_elem_ptr(tilted, tr.y + tr.height, tr.x - tr.height)
                    hidfeature.rect[k].weight = feature->rect[k].weight * correction_ratio
                    if( k == 0 )
                        area0 = tr.width * tr.height
                    else
                        sum0 += hidfeature.rect[k].weight * tr.width * tr.height
                hidfeature.rect[0].weight = -sum0/area0

```
---
```python
function load_cascade_cart( input_cascade, n, orig_window_size )
    cascade = create_classifier_cascade(n)
    cascade.orig_window_size = orig_window_size
    for i = 0 to n - 1
        threshold = 0;
        stage = input_cascade[i];
        dl = 0;
        parent = -1;
        next = -1;
        cascade.stage_classifier[i].count = count;
        cascade.stage_classifier[i].classifier = allocate( count * sizeof(cascade.stage_classifier[i].classifier[0]))
        for j = 0 to count - 1
            classifier = cascade.stage_classifier[i].classifier + j
            k, rects = 0
            stage += dl
            classifier.haar_feature = allocate( classifier.count * ( sizeof( classifier->haar_feature ) +
                                      sizeof( classifier->threshold ) +
                                      sizeof( classifier->left ) +
                                      sizeof( classifier->right ) ) +
                (classifier.count + 1) * sizeof( classifier.alpha ) )
            classifier.threshold = classifier.haar_feature + classifier.count
            classifier.left = classifier.threshold + classifier.count
            classifier.right = classifier.left + classifier.count
            classifier.alpha = classifier.right + classifier.count
            for l = 0 to classifier.count - 1
                stage += dl
                for k = 0 to rects - 1
                    band = 0
                    stage += dl
                    classifier.haar_feature[l].rect[k].r = r
                stage += dl
                classifier.haar_feature[l].tilted = strncmp( str, "tilted", 6 ) == 0
                for k = 0 to rects - 1
                    clear( classifier->haar_feature[l].rect + k, 0, sizeof(classifier->haar_feature[l].rect[k]))
                stage += dl
            for l = 0 to classifier.count - 1
                stage += dl
        stage += dl
        cascade.stage_classifier[i].threshold = threshold
        cascade.stage_classifier[i].parent = parent
        cascade.stage_classifier[i].next = next
        cascade.stage_classifier[i].child = -1
        if( parent != -1 && cascade->stage_classifier[parent].child == -1 )
            cascade.stage_classifier[parent].child = i
    return cascade
```
---
```python
function release_classifier_cascade( _cascade )
    if( _cascade )
        for i = 0 to cascade.count - 1
            for j = 0 to cascade->stage_classifier[i].count - 1
                free( cascade.stage_classifier[i].classifier[j].haar_feature );
            free( cascade.stage_classifier[i].classifier )
        release_classifier_cascade( cascade->hid_cascade )
        free( _cascade )
```
