export function getAssetUrl(path: string): string {
  return `${import.meta.env.BASE_URL}${path.replace(/^\//, '')}`;
}

export function getImageUrl(imageName: string): string {
  if (!imageName) return '';
  return getAssetUrl(`images/${imageName}`);
}